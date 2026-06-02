#ifndef EMBR_TRANSPORT_H_
#define EMBR_TRANSPORT_H_

/**
 * @file embr_transport.h
 * @brief Application-level transport for inferred embr commands.
 *
 * Validates inferred commands and forwards actionable commands to the active
 * transport backend.
 */

#include "embr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the embr transport module.
 *
 * Initializes the active transport backend before commands are sent.
 * Idempotent and safe to call more than once.
 *
 * @return 0 on success, or negative error code on failure
 */
int embr_transport_init(void);

/**
 * @brief Send an embr command derived from local inference.
 *
 * Actionable commands (EMBR_GLOW, EMBR_FLIP, EMBR_SLEEP) are forwarded to the
 * active transport backend. UNKNOWN and NOISE are intentionally ignored and
 * return 0. EMBR_RESET and invalid commands return -EINVAL. Errors returned by
 * the active transport backend are propagated to the caller.
 *
 * @param command embr command to handle
 * @return 0 on success, or negative error code on failure
 */
int embr_transport_send_command(embr_command command);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_TRANSPORT_H_ */
