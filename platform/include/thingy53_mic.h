#ifndef THINGY53_MIC_H_
#define THINGY53_MIC_H_

/**
 * @file thingy53_mic.h
 * @brief Thingy53 DMIC control API.
 *
 * High-level interface for initializing and toggling the Thingy53 DMIC.
 * All functions return 0 on success or -errno on failure.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_init(void);

/**
 * @brief Start the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_start(void);

/**
 * @brief Stop the Thingy53 DMIC.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_mic_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_H_ */
