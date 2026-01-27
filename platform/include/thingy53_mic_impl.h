#ifndef THINGY53_MIC_IMPL_H_
#define THINGY53_MIC_IMPL_H_

/**
 * @file thingy53_mic_impl.h
 * @brief Thingy53 DMIC implementation helper.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Thingy53 DMIC.
 *
 * Idempotent when already initialized and not streaming. Call deinit to
 * reapply configuration changes.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_init_impl(void);

/**
 * @brief Start the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_start_impl(void);

/**
 * @brief Stop the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_stop_impl(void);

/**
 * @brief Deinitialize the Thingy53 DMIC implementation state.
 *
 * Clears initialization/streaming state so init can apply new configuration.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_deinit_impl(void);

/**
 * @brief Reset the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_reset_impl(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_IMPL_H_ */
