#ifndef THINGY53_MIC_H_
#define THINGY53_MIC_H_

/**
 * @file thingy53_mic.h
 * @brief Thingy53 DMIC control API.
 *
 * High-level interface for initializing and controlling the Thingy53 DMIC.
 * All functions return 0 on success or -errno on failure.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Thingy53 DMIC.
 *
 * Idempotent when already initialized and not streaming. Call deinit to
 * reapply configuration changes.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_init(void);

/**
 * @brief Start the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_start(void);

/**
 * @brief Stop the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_stop(void);

/**
 * @brief Deinitialize the Thingy53 DMIC state.
 *
 * Clears initialization/streaming state so init can apply new configuration.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_deinit(void);

/**
 * @brief Reset the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_reset(void);

/**
 * @brief Read a released Thingy53 DMIC buffer.
 *
 * @param buffer Output pointer to released PCM buffer
 * @param size Output size of released PCM buffer in bytes
 * @param timeout Timeout in milliseconds, or SYS_FOREVER_MS
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_read(void **buffer, size_t *size, int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_H_ */
