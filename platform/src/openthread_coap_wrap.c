#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <net/coap_utils.h>
#include <openthread/instance.h>
#include <openthread/thread.h>
#include <zephyr/net/openthread.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/atomic.h>

#include "openthread_coap_wrap.h"

static bool initialized = false;
/* Updated from OpenThread context and read from application context. */
static atomic_t attached = ATOMIC_INIT(0);

static bool role_is_attached(otDeviceRole role) {
  switch (role) {
  case OT_DEVICE_ROLE_CHILD:
  case OT_DEVICE_ROLE_ROUTER:
  case OT_DEVICE_ROLE_LEADER:
    return true;

  case OT_DEVICE_ROLE_DISABLED:
  case OT_DEVICE_ROLE_DETACHED:
  default:
    return false;
  }
}

static void on_thread_state_changed(otChangedFlags flags,
                                    struct openthread_context *ot_context,
                                    void *user_data) {
  (void)user_data;

  if (!(flags & OT_CHANGED_THREAD_ROLE)) {
    return;
  }

  atomic_set(&attached,
             role_is_attached(otThreadGetDeviceRole(ot_context->instance)));
}

static struct openthread_state_changed_cb ot_state_changed_cb = {
    .state_changed_cb = on_thread_state_changed};

int openthread_coap_wrap_init(void) {
  struct openthread_context *ot_context = openthread_get_default_context();

  if (initialized) {
    return 0;
  }

  if (!ot_context) {
    return -ENODEV;
  }

  int err =
      openthread_state_changed_cb_register(ot_context, &ot_state_changed_cb);
  if (err) {
    return err;
  }

  err = openthread_start(ot_context);
  if (err) {
    (void)openthread_state_changed_cb_unregister(ot_context,
                                                 &ot_state_changed_cb);
    return err;
  }

  openthread_api_mutex_lock(ot_context);
  atomic_set(&attached,
             role_is_attached(otThreadGetDeviceRole(ot_context->instance)));
  openthread_api_mutex_unlock(ot_context);

  coap_init(AF_INET6, NULL);

  initialized = true;

  return 0;
}

int openthread_coap_wrap_send_put(uint16_t port,
                                  const char *const *uri_path_options,
                                  uint8_t *payload, uint16_t payload_size) {
  if (!initialized) {
    return -EACCES;
  }

  if (!atomic_get(&attached)) {
    return -ENOTCONN;
  }

  /* Thread realm-local all-nodes multicast address: ff03::1. */
  const struct sockaddr_in6 multicast_addr = {
      .sin6_family = AF_INET6,
      .sin6_port = htons(port),
      .sin6_addr.s6_addr = {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
      .sin6_scope_id = 0U};

  return coap_send_request(COAP_METHOD_PUT,
                           (const struct sockaddr *)&multicast_addr,
                           uri_path_options, payload, payload_size, NULL);
}
