#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

#include "embr_app.h"
#include "embr_error_id.h"
#include "thingy53_led.h"
#include "thingy53_mic.h"

LOG_MODULE_REGISTER(embr_app);

// Debug: Toggles LED pulses on PDM buffer release
#define EMBR_LED_PULSE 0

#define LED_250_MS_COLOR THINGY53_LED_RED
#define LED_1_S_COLOR THINGY53_LED_GREEN

#if EMBR_LED_PULSE
struct led_work {
  struct k_work_delayable off_work;
  thingy53_led_color_t color;
};

static struct led_work led_blink;

static void led_off_work_handler(struct k_work *work) {
  struct led_work *lw = CONTAINER_OF(work, struct led_work, off_work.work);
  thingy53_led_toggle(lw->color);
}
#endif /* EMBR_LED_PULSE */

extern struct k_sem pdm_smphr;
static bool app_initialized;

embr_err_t embr_app_init(void) {
  embr_err_t err = 0;

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

#if EMBR_LED_PULSE
  k_work_init_delayable(&led_blink.off_work, led_off_work_handler);
#endif

  app_initialized = true;
  return EMBR_OK;
}

embr_err_t embr_app_start(void) {
  __ASSERT(app_initialized, "embr_app_start called before embr_app_init");
  if (!app_initialized) {
    return -EINVAL;
  }

  embr_err_t err = thingy53_mic_start();
  if (err) {
    return err;
  }

#if EMBR_LED_PULSE
  uint8_t slice_count = 0;
#endif

  while (1) {
    k_sem_take(&pdm_smphr, K_FOREVER);

#if EMBR_LED_PULSE
    slice_count++;
    if (slice_count == 4) {
      slice_count = 0;
      led_blink.color = LED_1_S_COLOR;
    } else {
      led_blink.color = LED_250_MS_COLOR;
    }
    thingy53_led_toggle(led_blink.color);
    k_work_schedule(&led_blink.off_work, K_MSEC(10));
#endif
  }

  return EMBR_OK;
}
