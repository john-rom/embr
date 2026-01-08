#ifndef EMBR_APP_H_
#define EMBR_APP_H_

/**
 * @file embr_app.h
 * @brief Ember application lifecycle API
 *
 * Return convention:
 * - 0 on success
 * - Negative error code on failure (prefer -errno)
 * - Ember-reserved errors are < EMBR_ERR_BASE
 */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int embr_err_t;

#define EMBR_OK 0
#define EMBR_ERR_BASE (-10000)
#define EMBR_ERR_FAIL (EMBR_ERR_BASE - 1)

static inline bool embr_ok(embr_err_t e) { return e == 0; }
static inline bool embr_fail(embr_err_t e) { return e < 0; }

/** Initialize application state. Call once before embr_app_start(). */
embr_err_t embr_app_init(void);

/** Start the application. Call after embr_app_init(). */
embr_err_t embr_app_start(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_APP_H_ */
