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
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_init_impl(void);

/**
 * @brief Start the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_start_impl(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_IMPL_H_ */
