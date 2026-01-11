#ifndef EMBR_APP_H_
#define EMBR_APP_H_

/**
 * @file embr_app.h
 * @brief Ember application lifecycle API.
 *
 * Return convention:
 * - 0 (EMBR_OK) on success
 * - Negative error code on failure; -errno when applicable
 */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int embr_err_t;

#define EMBR_OK 0

static inline bool embr_ok(embr_err_t e) { return e == 0; }
static inline bool embr_fail(embr_err_t e) { return e < 0; }

/**
 * @brief Initialize application state and dependencies.
 *
 * Sets up internal state and any subsystems required before the main
 * application loop starts.
 *
 * @return 0 (EMBR_OK) on success; negative error code on failure.
 */
embr_err_t embr_app_init(void);

/**
 * @brief Start the application runtime.
 *
 * Kicks off the main application flow after successful initialization.
 *
 * @return 0 (EMBR_OK) on success; no failure mode currently implemented.
 */
embr_err_t embr_app_start(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_APP_H_ */
