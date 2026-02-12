#ifndef EMBR_INFERENCE_H_
#define EMBR_INFERENCE_H_

/**
 * @file embr_inference.h
 * @brief Inference capture workflow for WOS-triggered audio.
 *
 * Manages DMIC capture slices, feeds Edge Impulse, and stops capture
 * after a full inference window is collected.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/** Capture state for the WOS session. */
enum capture_state {
  CAPTURE_IDLE = 0,
  CAPTURE_CAPTURING
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start Edge Impulse prediction for the current window.
 *
 * @return 0 on success, or -errno on failure
 */
int embr_inference_start(void);

/**
 * @brief Begin audio capture after a WOS trigger.
 *
 * Blocks until a WOS event is received, then starts DMIC capture.
 *
 * @return 0 on success, or -errno on failure
 */
int embr_inference_begin_audio_capture_after_wos(void);

/**
 * @brief Process the next WOS-triggered audio slice.
 *
 * Waits for the next released buffer, discards the first non-empty
 * startup buffer to flush stale data and initial transient noise,
 * then pushes PCM to the inference pipeline.
 *
 * @return 0 on success, or -errno on failure
 */
int embr_inference_process_wos_event_audio(void);

/**
 * @brief Convert and enqueue a PCM slice into the EI input buffer.
 *
 * @param pcm_buf Pointer to PCM buffer
 * @param pcm_size Size of PCM buffer in bytes
 * @return 0 on success, or -errno on failure
 */
int embr_inference_move_slice_to_pipeline(const void *pcm_buf, size_t pcm_size);

/**
 * @brief Stop capture once the inference window is full.
 *
 * @return 1 if the window was full and capture was stopped,
 *         0 if the window is not yet full,
 *         or -errno on failure
 */
int embr_inference_stop_audio_if_window_full(void);

/**
 * @brief Reset inference capture state for a clean restart.
 */
void embr_inference_reset_state(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_INFERENCE_H_ */
