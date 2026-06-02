#ifndef OPENTHREAD_COAP_WRAP_H_
#define OPENTHREAD_COAP_WRAP_H_

/**
 * @file openthread_coap_wrap.h
 * @brief OpenThread CoAP wrapper API for unit-test compatibility.
 *
 * Encapsulates NCS OpenThread and socket-based CoAP utility calls so
 * application code remains independent of SDK networking types.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize OpenThread and the socket-based CoAP utility.
 *
 * Registers Thread attachment-state tracking and starts OpenThread networking.
 * Idempotent and safe to call more than once.
 *
 * @return 0 on success, or negative error code on failure
 */
int openthread_coap_wrap_init(void);

/**
 * @brief Send a multicast CoAP PUT request to local Thread nodes.
 *
 * Sends a non-confirmable request to the realm-local all-nodes multicast
 * address. Calling this function before successful initialization returns
 * -EACCES. Calling it while detached from Thread returns -ENOTCONN.
 *
 * @param port Destination UDP port in host byte order
 * @param uri_path_options NULL-terminated URI path segment array
 * @param payload Request payload
 * @param payload_size Request payload size in bytes
 * @return Non-negative byte count on success, or negative error code on failure
 */
int openthread_coap_wrap_send_put(uint16_t port,
                                  const char *const *uri_path_options,
                                  uint8_t *payload, uint16_t payload_size);

#ifdef __cplusplus
}
#endif

#endif /* OPENTHREAD_COAP_WRAP_H_ */
