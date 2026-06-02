#include "openthread_coap_wrap_mock.h"

#include <stddef.h>

int mock_openthread_coap_wrap_init_return_value = 0;
int mock_openthread_coap_wrap_init_call_count = 0;
int mock_openthread_coap_wrap_send_put_return_value = 0;
int mock_openthread_coap_wrap_send_put_call_count = 0;
uint16_t mock_openthread_coap_wrap_send_put_last_port = 0;
const char *const *mock_openthread_coap_wrap_send_put_last_uri_path_options =
    NULL;
uint8_t mock_openthread_coap_wrap_send_put_last_payload = 0;
uint16_t mock_openthread_coap_wrap_send_put_last_payload_size = 0;

void openthread_coap_wrap_mock_reset(void) {
  mock_openthread_coap_wrap_init_return_value = 0;
  mock_openthread_coap_wrap_init_call_count = 0;
  mock_openthread_coap_wrap_send_put_return_value = 0;
  mock_openthread_coap_wrap_send_put_call_count = 0;
  mock_openthread_coap_wrap_send_put_last_port = 0;
  mock_openthread_coap_wrap_send_put_last_uri_path_options = NULL;
  mock_openthread_coap_wrap_send_put_last_payload = 0;
  mock_openthread_coap_wrap_send_put_last_payload_size = 0;
}

int openthread_coap_wrap_init(void) {
  mock_openthread_coap_wrap_init_call_count++;
  return mock_openthread_coap_wrap_init_return_value;
}

int openthread_coap_wrap_send_put(uint16_t port,
                                  const char *const *uri_path_options,
                                  uint8_t *payload, uint16_t payload_size) {
  mock_openthread_coap_wrap_send_put_call_count++;
  mock_openthread_coap_wrap_send_put_last_port = port;
  mock_openthread_coap_wrap_send_put_last_uri_path_options = uri_path_options;
  mock_openthread_coap_wrap_send_put_last_payload =
      payload && payload_size ? payload[0] : 0;
  mock_openthread_coap_wrap_send_put_last_payload_size = payload_size;
  return mock_openthread_coap_wrap_send_put_return_value;
}
