#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

#include "dmic_wrap.h"
#include "embr_app.h"
#include "embr_error_id.h"
#include "thingy53_led.h"
#include "thingy53_mic.h"
#include "vm3011.h"

#define NO_SLICES 0
#define SAMPLES_PER_INFERENCE_WINDOW DMIC_WRAP_SAMPLE_RATE_HZ
#define SLICES_PER_INFERENCE_WINDOW                                            \
  (SAMPLES_PER_INFERENCE_WINDOW / CONFIG_VM3011_PDM_BUFFER_SIZE)
#define FIRST_BUFFER_MIN_MS                                                    \
  ((CONFIG_VM3011_PDM_BUFFER_SIZE * 1000) / DMIC_WRAP_SAMPLE_RATE_HZ - 50)

BUILD_ASSERT(SLICES_PER_INFERENCE_WINDOW <= UINT8_MAX,
             "slices_just_captured must fit in uint8_t");

LOG_MODULE_REGISTER(embr_app);

K_SEM_DEFINE(wos_smphr, 0, 1);
K_SEM_DEFINE(pdm_smphr, 0, 1);

void vm3011_wos_triggered_hook(void) { k_sem_give(&wos_smphr); }
void vm3011_buffer_released_hook(void) { k_sem_give(&pdm_smphr); }
void vm3011_error_hook(vm3011_error_t err) {
  switch (err) {
  case VM3011_ERR_PDM_OVERFLOW:
    embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
    break;
  case VM3011_ERR_PDM_SET_BUFFER:
    embr_error_report(EMBR_ERR_ID_PDM_SET_BUFFER);
    break;
  default:
    break;
  }
}

struct led_work {
  struct k_work off_work;
  thingy53_led_color_t color;
};

static struct led_work led_blink;

static void led_off_work_handler(struct k_work *work) {
  struct led_work *lw = CONTAINER_OF(work, struct led_work, off_work);
  thingy53_led_toggle(lw->color);
}

static bool app_initialized = false;

enum capture_state { CAPTURE_IDLE = 0, CAPTURE_CAPTURING };

embr_err_t embr_app_init(void) {
  embr_err_t err = EMBR_OK;

  err = thingy53_led_init();
  if (err) {
    LOG_ERR("Thingy53 LED init failed: %d", err);
    return err;
  }

  err = thingy53_mic_init();
  if (err) {
    LOG_ERR("Thingy53 DMIC init failed: %d", err);
    return err;
  }

  k_work_init(&led_blink.off_work, led_off_work_handler);

  app_initialized = true;

  return EMBR_OK;
}

embr_err_t embr_app_start(void) {
  __ASSERT(app_initialized, "embr_app_start called before embr_app_init");
  if (!app_initialized) {
    return -EINVAL;
  }

  embr_err_t err = EMBR_OK;

  static enum capture_state capture_state = CAPTURE_IDLE;
  static uint8_t slices_just_captured = 0;
  static bool skip_first_slice = false;
  static int64_t capture_start_ms = 0;

  while (1) {
    if (capture_state == CAPTURE_IDLE) {
      slices_just_captured = NO_SLICES;
      k_sem_take(&wos_smphr, K_FOREVER);

      led_blink.color = THINGY53_LED_GREEN;
      thingy53_led_toggle(led_blink.color);

      err = thingy53_mic_start();
      if (err) {
        return err;
      }
      // First buffer after start can contain partial or bad data;
      // Discard it to get a proper window
      skip_first_slice = true;
      capture_start_ms = k_uptime_get();
      capture_state = CAPTURE_CAPTURING;
    }

    if (capture_state == CAPTURE_CAPTURING) {
      k_sem_take(&pdm_smphr, K_FOREVER);
      if (skip_first_slice) {
        int64_t elapsed_ms = k_uptime_get() - capture_start_ms;
        skip_first_slice = false;
        if (elapsed_ms < FIRST_BUFFER_MIN_MS) {
          continue;
        }
        // First buffer was full-length; count it.
        slices_just_captured++;
      } else {
        // Move to inference pipeline
        slices_just_captured++;
      }

      if (slices_just_captured >= SLICES_PER_INFERENCE_WINDOW) {
        err = thingy53_mic_stop();
        if (err) {
          return err;
        }
        k_work_submit(&led_blink.off_work);
        err = thingy53_mic_reset();
        if (err) {
          return err;
        }
        slices_just_captured = 0;
        capture_state = CAPTURE_IDLE;
      }
    }
  }

  return EMBR_OK;
}
