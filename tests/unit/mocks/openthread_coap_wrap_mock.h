#ifndef OPENTHREAD_COAP_WRAP_MOCK_H_
#define OPENTHREAD_COAP_WRAP_MOCK_H_

/**
 * @file openthread_coap_wrap_mock.h
 * @brief Mock helpers for the OpenThread CoAP wrapper API.
 */

#include "openthread_coap_wrap.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_openthread_coap_wrap_init_return_value;
extern int mock_openthread_coap_wrap_init_call_count;
extern int mock_openthread_coap_wrap_send_put_return_value;
extern int mock_openthread_coap_wrap_send_put_call_count;
extern uint16_t mock_openthread_coap_wrap_send_put_last_port;
extern const char *const *mock_openthread_coap_wrap_send_put_last_uri_path_options;
extern uint8_t mock_openthread_coap_wrap_send_put_last_payload;
extern uint16_t mock_openthread_coap_wrap_send_put_last_payload_size;

void openthread_coap_wrap_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* OPENTHREAD_COAP_WRAP_MOCK_H_ */
