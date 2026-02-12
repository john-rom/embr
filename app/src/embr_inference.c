#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

#include "dmic_wrap.h"
#include "ei_wrap.h"
#include "embr_app.h"
#include "embr_inference.h"
#include "kernel_wrap.h"
#include "thingy53_mic.h"

#define EI_WRAP_SAMPLES_PER_SLICE CONFIG_VM3011_PDM_BUFFER_SIZE
#define EI_WRAP_SAMPLES_PER_INFERENCE_WINDOW DMIC_WRAP_SAMPLE_RATE_HZ
#define EI_WRAP_SLICES_PER_INFERENCE_WINDOW                                    \
  (EI_WRAP_SAMPLES_PER_INFERENCE_WINDOW / EI_WRAP_SAMPLES_PER_SLICE)
#define EMBR_PDM_TIMEOUT_MS 500
#define EMBR_PDM_INVALID_FRAME_THRESHOLD 3

BUILD_ASSERT(EI_WRAP_SLICES_PER_INFERENCE_WINDOW <= UINT8_MAX,
             "slices_just_captured must fit in uint8_t");

static enum capture_state capture_state = CAPTURE_IDLE;
static uint8_t slices_just_captured = 0;
static bool skip_first_slice = false;
static uint8_t invalid_frame_count = 0;
static float ei_input_buf[EI_WRAP_SAMPLES_PER_SLICE];

int embr_inference_start(void) { return ei_wrap_start_prediction(0, 0); }

int embr_inference_begin_audio_capture_after_wos(void) {
  if (capture_state == CAPTURE_IDLE) {
    slices_just_captured = 0;
    invalid_frame_count = 0;
    int err = kernel_wrap_sem_take_forever(embr_app_wos_sem());
    if (err) {
      return err;
    }

    err = embr_app_led_toggle_on_capture_start();
    if (err) {
      return err;
    }

    err = thingy53_mic_start();
    if (err) {
      return err;
    }
    // Discard first non-empty startup buffer to flush stale data and
    // initial transient noise.
    skip_first_slice = true;
    capture_state = CAPTURE_CAPTURING;
  }

  return 0;
}

int embr_inference_discard_startup_slice(void) {
  void *pcm_buf = NULL;
  size_t pcm_size = 0;

  int err = thingy53_mic_read(&pcm_buf, &pcm_size, 0); // timeout ms
  if (err) {
    return err;
  }
  if (pcm_buf == NULL || pcm_size == 0) {
    return 0;
  }
  skip_first_slice = false;
  return 0;
}

int embr_inference_move_slice_to_pipeline(const void *pcm_buf,
                                          size_t pcm_size) {
  __ASSERT(pcm_buf != NULL, "pcm_buf is null");
  if (pcm_buf == NULL) {
    return -EINVAL;
  }
  if (pcm_size == 0) {
    return -EINVAL;
  }
  if ((pcm_size % sizeof(int16_t)) != 0) {
    return -EINVAL;
  }

  const int16_t *pcm = (const int16_t *)pcm_buf;
  size_t sample_count = pcm_size / sizeof(int16_t);
  if (sample_count > EI_WRAP_SAMPLES_PER_SLICE) {
    return -EINVAL;
  }
  for (size_t i = 0; i < sample_count; i++) {
    ei_input_buf[i] = (float)pcm[i];
  }
  int err = ei_wrap_add_data(ei_input_buf, sample_count);
  if (err) {
    return err;
  }
  slices_just_captured++;
  return 0;
}

int embr_inference_process_wos_event_audio(void) {
  int err = 0;
  if (capture_state == CAPTURE_CAPTURING) {
    err = kernel_wrap_sem_take_timeout_ms(embr_app_pdm_sem(),
                                          EMBR_PDM_TIMEOUT_MS);
    if (err) {
      if (err == -EAGAIN) {
        return -ETIMEDOUT;
      }
      return err;
    }
    if (skip_first_slice) {
      err = embr_inference_discard_startup_slice();
      if (err) {
        return err;
      }
      if (skip_first_slice) {
        // Keep skipping until first non-empty startup buffer is discarded.
        return 0;
      }
    } else {
      void *pcm_buf = NULL;
      size_t pcm_size = 0;

      err = thingy53_mic_read(&pcm_buf, &pcm_size, 0); // timeout ms
      if (err) {
        return err;
      }
      if (pcm_buf && pcm_size) {
        size_t expected_bytes = EI_WRAP_SAMPLES_PER_SLICE * sizeof(int16_t);
        if (pcm_size != expected_bytes) {
          invalid_frame_count++;
          if (invalid_frame_count >= EMBR_PDM_INVALID_FRAME_THRESHOLD) {
            // Reuse timeout recovery policy for persistent malformed frames.
            return -ETIMEDOUT;
          }
          return 0;
        }
        err = embr_inference_move_slice_to_pipeline(pcm_buf, pcm_size);
        if (err) {
          return err;
        }
        invalid_frame_count = 0;
      }
    }
  }

  return 0;
}

int embr_inference_stop_audio_if_window_full(void) {
  if (slices_just_captured >= EI_WRAP_SLICES_PER_INFERENCE_WINDOW) {
    int err = thingy53_mic_stop();
    if (err) {
      return err;
    }
    err = embr_app_led_toggle_on_capture_end_async();
    if (err) {
      return err;
    }
    err = thingy53_mic_reset();
    if (err) {
      return err;
    }
    slices_just_captured = 0;
    capture_state = CAPTURE_IDLE;
    return 1;
  }

  return 0;
}

void embr_inference_reset_state(void) {
  capture_state = CAPTURE_IDLE;
  slices_just_captured = 0;
  skip_first_slice = false;
  invalid_frame_count = 0;
}
