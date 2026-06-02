#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include "embr_coap_client.h"
#include "openthread_coap_wrap.h"

static const char *const light_uri_path[] = {EMBR_COAP_LIGHT_URI_PATH, NULL};

int embr_coap_client_init(void) {
  return openthread_coap_wrap_init();
}

int embr_coap_client_send(enum embr_coap_light_command command) {
  uint8_t payload = (uint8_t)command;

  switch (command) {
  case EMBR_COAP_LIGHT_SLEEP:
  case EMBR_COAP_LIGHT_GLOW:
  case EMBR_COAP_LIGHT_FLIP:
    break;

  default:
    return -EINVAL;
  }

  int ret = openthread_coap_wrap_send_put(
      EMBR_COAP_PORT, light_uri_path, &payload, sizeof(payload));

  return ret < 0 ? ret : 0;
}
