#include "embr_coap_client_mock.h"

int mock_embr_coap_client_init_return_value = 0;
int mock_embr_coap_client_init_call_count = 0;
int mock_embr_coap_client_send_return_value = 0;
int mock_embr_coap_client_send_call_count = 0;
enum embr_coap_light_command mock_embr_coap_client_send_last_command =
    (enum embr_coap_light_command)-1;

void embr_coap_client_mock_reset(void) {
  mock_embr_coap_client_init_return_value = 0;
  mock_embr_coap_client_init_call_count = 0;
  mock_embr_coap_client_send_return_value = 0;
  mock_embr_coap_client_send_call_count = 0;
  mock_embr_coap_client_send_last_command =
      (enum embr_coap_light_command)-1;
}

int embr_coap_client_init(void) {
  mock_embr_coap_client_init_call_count++;
  return mock_embr_coap_client_init_return_value;
}

int embr_coap_client_send(enum embr_coap_light_command command) {
  mock_embr_coap_client_send_call_count++;
  mock_embr_coap_client_send_last_command = command;
  return mock_embr_coap_client_send_return_value;
}
