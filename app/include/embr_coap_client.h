#ifndef EMBR_COAP_CLIENT_H_
#define EMBR_COAP_CLIENT_H_

/**
 * @file embr_coap_client.h
 * @brief CoAP client adapter for embr light commands.
 *
 * Sends embr light commands through the platform OpenThread CoAP wrapper while
 * keeping SDK-specific networking details outside application code.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "embr_coap_protocol.h"

/**
 * @brief Initialize the embr CoAP client.
 *
 * Initializes the platform OpenThread CoAP wrapper. Idempotent and safe to call
 * more than once.
 *
 * @return 0 on success, or negative error code on failure
 */
int embr_coap_client_init(void);

/**
 * @brief Send an embr light command to local server nodes.
 *
 * Sends a non-confirmable multicast CoAP PUT request to the embr light
 * endpoint. Calling this function before successful initialization returns
 * -EACCES. Calling it while detached from Thread returns -ENOTCONN. Invalid
 * commands return -EINVAL.
 *
 * @param command Light command to transmit
 * @return 0 on success, or negative error code on failure
 */
int embr_coap_client_send(enum embr_coap_light_command command);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_COAP_CLIENT_H_ */
