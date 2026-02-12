#ifndef THINGY53_MIC_IMPL_H_
#define THINGY53_MIC_IMPL_H_

/**
 * @file thingy53_mic_impl.h
 * @brief Thingy53 DMIC implementation helper.
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
int thingy53_mic_init_impl(void);

/**
 * @brief Start the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_start_impl(void);

/**
 * @brief Stop the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_stop_impl(void);

/**
 * @brief Deinitialize the Thingy53 DMIC implementation state.
 *
 * Clears initialization/streaming state so init can apply new configuration.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_deinit_impl(void);

/**
 * @brief Reset the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_reset_impl(void);

/**
 * @brief Read a released Thingy53 DMIC buffer.
 *
 * @param buffer Pointer to the released buffer address
 * @param size Pointer to the released buffer size
 * @param timeout Timeout in milliseconds to wait in case audio is not yet
 * received, or SYS_FOREVER_MS.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_mic_read_impl(void **buffer, size_t *size, int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_IMPL_H_ */
