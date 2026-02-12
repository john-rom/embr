#ifndef EMBR_ERROR_ID_H_
#define EMBR_ERROR_ID_H_

/**
 * @file embr_error_id.h
 * @brief Error identifiers for asynchronous reporting.
 *
 * Error IDs are used for deferred reporting in non-blocking contexts.
 */

#include <stdint.h>

/** Error identifiers used with embr_error_report(). */
typedef enum {
  /** LED toggle operation failed. */
  EMBR_ERR_ID_LED_TOGGLE = 0,
  /** VM3011 reported PDM overflow. */
  EMBR_ERR_ID_PDM_OVERFLOW,
  /** VM3011 failed to set PDM buffer. */
  EMBR_ERR_ID_PDM_SET_BUFFER,
  /** Sentinel count of defined error IDs. */
  EMBR_ERR_ID_COUNT,
} embr_error_id_t;

/** Deferred error callback signature. */
typedef void (*embr_error_handler_t)(embr_error_id_t id);

/**
 * @brief Internal deferred-error transport diagnostics.
 *
 * Counts saturate at UINT8_MAX and do not wrap.
 */
struct embr_error_stats {
  /** Number of work-init failures (saturates at UINT8_MAX). */
  uint8_t work_init_fail_count;
  /** Number of work-submit failures (saturates at UINT8_MAX). */
  uint8_t work_submit_fail_count;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Report an error identifier for deferred handling.
 *
 * Safe to call from callbacks where logging is not appropriate.
 * Deferred transport submit failures are retried internally up to a bounded
 * limit; if submission still fails, pending IDs are dropped.
 *
 * @param id Error identifier to report
 */
void embr_error_report(embr_error_id_t id);

/**
 * @brief Register a handler for deferred error IDs.
 *
 * When set, the handler is invoked from the error workqueue context
 * for each reported error ID. If no handler is registered, reported
 * errors are dropped.
 *
 * @param handler Handler to register, or NULL to clear
 */
void embr_error_register_handler(embr_error_handler_t handler);

/**
 * @brief Convert an error ID to a string.
 *
 * @param id Error identifier
 * @return String representation of the error ID
 */
const char *embr_error_id_to_str(embr_error_id_t id);

/**
 * @brief Read internal deferred-error transport diagnostics.
 *
 * @param out_stats Output stats snapshot
 */
void embr_error_get_stats(struct embr_error_stats *out_stats);

/**
 * @brief Reset internal deferred-error transport diagnostics.
 */
void embr_error_reset_stats(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_ERROR_ID_H_ */
