#ifndef EMBR_APP_H_
#define EMBR_APP_H_

/**
 * @file embr_app.h
 * @brief embr application runtime shell API.
 *
 * This module is the runtime shell: it owns Zephyr-facing orchestration
 * (init/start sequencing, hooks, and runtime loop wiring). Pure transition
 * decisions are delegated to embr_app_logic.
 *
 * Return convention:
 * - 0 on success
 * - Negative error code on failure; -errno when applicable
 */

struct k_sem;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize application state and dependencies.
 *
 * Sets up internal state and any subsystems required before the main
 * application loop starts.
 *
 * @return 0 on success; negative error code on failure
 */
int embr_app_init(void);

/**
 * @brief Start the application runtime.
 *
 * Kicks off the main application flow after successful initialization.
 *
 * @return 0 on success, or negative error code on failure
 */
int embr_app_start(void);

/**
 * @brief Turn on the capture indicator LED.
 *
 * @return 0 on success, or -errno on failure
 */
int embr_app_led_toggle_on_capture_start(void);

/**
 * @brief Defer capture-end indicator LED toggle via work queue.
 *
 * @return 0 on success, or -errno on failure
 */
int embr_app_led_toggle_on_capture_end_async(void);

/**
 * @brief Get the WOS semaphore used for capture synchronization.
 *
 * @return Pointer to the WOS semaphore
 */
struct k_sem *embr_app_wos_sem(void);

/**
 * @brief Get the PDM semaphore used for buffer release synchronization.
 *
 * @return Pointer to the PDM semaphore
 */
struct k_sem *embr_app_pdm_sem(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_APP_H_ */
