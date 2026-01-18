#include "thingy53_led_impl.h"
#include "thingy53_led_specs.h"

#include <errno.h>

#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

#include "gpio_wrap.h"

int thingy53_led_init_impl(void) {
  const struct gpio_dt_spec *leds[] = {
      thingy53_led_get_red(),
      thingy53_led_get_green(),
      thingy53_led_get_blue(),
  };

  for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
    __ASSERT(leds[i] != NULL, "gpio spec is null");
    if (!gpio_wrap_is_ready_dt(leds[i])) {
      return -ENODEV;
    }

    int err = gpio_wrap_pin_configure_dt_output_inactive(leds[i]);
    if (err) {
      return err;
    }
  }

  return 0;
}

int thingy53_led_toggle_impl(thingy53_led_color_t color) {
  const struct gpio_dt_spec *spec = NULL;

  switch (color) {
  case THINGY53_LED_RED:
    spec = thingy53_led_get_red();
    break;
  case THINGY53_LED_GREEN:
    spec = thingy53_led_get_green();
    break;
  case THINGY53_LED_BLUE:
    spec = thingy53_led_get_blue();
    break;
  default:
    __ASSERT(false, "invalid LED color");
    return -EINVAL;
  }

  __ASSERT(spec != NULL, "gpio spec is null");
  int err = gpio_wrap_pin_toggle_dt(spec);
  if (err) {
    return err;
  }

  return 0;
}
