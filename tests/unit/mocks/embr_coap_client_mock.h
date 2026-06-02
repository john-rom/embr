#ifndef EMBR_COAP_CLIENT_MOCK_H_
#define EMBR_COAP_CLIENT_MOCK_H_

/**
 * @file embr_coap_client_mock.h
 * @brief Mock helpers for the embr CoAP client API.
 */

#include "embr_coap_client.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_embr_coap_client_init_return_value;
extern int mock_embr_coap_client_init_call_count;
extern int mock_embr_coap_client_send_return_value;
extern int mock_embr_coap_client_send_call_count;
extern enum embr_coap_light_command mock_embr_coap_client_send_last_command;

void embr_coap_client_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_COAP_CLIENT_MOCK_H_ */
