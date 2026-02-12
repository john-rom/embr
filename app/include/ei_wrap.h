#ifndef EI_WRAP_H_
#define EI_WRAP_H_

/**
 * @file ei_wrap.h
 * @brief Thin wrapper for Nordic's Edge Impulse wrapper API.
 *
 * This interface forwards calls to the NCS-provided Edge Impulse wrapper to
 * keep the app surface minimal and mockable.
 */

#include <stddef.h>

/**
 * @brief Edge Impulse inference result callback.
 *
 * @param err 0 on success, negative error code on failure
 */
typedef void (*ei_wrapper_result_ready_cb)(int err);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Edge Impulse wrapper.
 *
 * @param cb Callback invoked when inference results are ready
 * @return 0 on success, or -errno on failure
 */
int ei_wrap_init(ei_wrapper_result_ready_cb cb);

/**
 * @brief Add raw audio data to the Edge Impulse buffer.
 *
 * @param data Pointer to float PCM samples
 * @param data_size Number of samples in @p data
 * @return 0 on success, or -errno on failure
 */
int ei_wrap_add_data(const float *data, size_t data_size);

/**
 * @brief Start or advance an inference prediction window.
 *
 * @param window_shift Number of windows the input window is shifted
 * @param frame_shift Number of frames the input window is shifted
 * @return 0 on success, or -errno on failure
 */
int ei_wrap_start_prediction(size_t window_shift, size_t frame_shift);

/**
 * @brief Get the next classification result from the EI wrapper.
 *
 * @param label Output label string
 * @param value Output probability value
 * @param idx Output index (optional)
 * @return 0 on success, -ENOENT when no more results are available, or -errno
 */
int ei_wrap_get_next_classification_result(const char **label, float *value,
                                           size_t *idx);

/**
 * @brief Check whether the classifier model provides anomaly output.
 *
 * @return 1 when anomaly output is available, 0 otherwise
 */
int ei_wrap_classifier_has_anomaly(void);

/**
 * @brief Get anomaly score from the current inference result.
 *
 * @param anomaly Output anomaly score
 * @return 0 on success, or -errno on failure
 */
int ei_wrap_get_anomaly(float *anomaly);

#ifdef __cplusplus
}
#endif

#endif /* EI_WRAP_H_ */
