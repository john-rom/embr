#ifndef EMBR_ERROR_ID_H_
#define EMBR_ERROR_ID_H_

/**
 * @file embr_error_id.h
 * @brief Error identifiers for asynchronous reporting.
 *
 * Error IDs are used for deferred reporting in non-blocking contexts.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Error identifiers used with embr_error_report(). */
typedef enum {
  EMBR_ERR_ID_LED_TOGGLE = 0,
  EMBR_ERR_ID_COUNT,
} embr_error_id_t;

/**
 * @brief Report an error identifier for deferred handling.
 *
 * Safe to call from callbacks where logging is not appropriate.
 *
 * @param id Error identifier to report.
 */
void embr_error_report(embr_error_id_t id);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_ERROR_ID_H_ */
