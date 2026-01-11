#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

#include "embr_app.h"
#include "embr_error_id.h"
#include "thingy53_led.h"

LOG_MODULE_REGISTER(embr_app);

#define EMBR_LED_START_DELAY_MS 100
#define EMBR_LED_BLINK_PERIOD_MS 500

static struct k_timer led_timer;
static bool app_initialized;

static void led_timer_handler(struct k_timer *timer) {
  (void)timer;
  embr_err_t err = thingy53_led_toggle(THINGY53_LED_GREEN);
  if (err) {
    embr_error_report(EMBR_ERR_ID_LED_TOGGLE);
  }
}

embr_err_t embr_app_init(void) {
  embr_err_t err = thingy53_led_init();
  if (err) {
    LOG_ERR("Thingy53 LED init failed: %d", err);
    return err;
  }

  k_timer_init(&led_timer, led_timer_handler, NULL);
  app_initialized = true;
  return EMBR_OK;
}

embr_err_t embr_app_start(void) {
  __ASSERT(app_initialized, "embr_app_start called before embr_app_init");
  k_timer_start(&led_timer, K_MSEC(EMBR_LED_START_DELAY_MS),
                K_MSEC(EMBR_LED_BLINK_PERIOD_MS));
  return EMBR_OK;
}
