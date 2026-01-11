#include "thingy53_led.h"

#include <errno.h>
#include <stddef.h>

#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

#include "thingy53_led_impl.h"
#include "thingy53_led_specs.h"

int thingy53_led_init(void) {
  const struct gpio_dt_spec *leds[] = {
      thingy53_led_get_red(),
      thingy53_led_get_green(),
      thingy53_led_get_blue(),
  };

  for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
    int err = thingy53_led_init_impl(leds[i]);
    if (err) {
      return err;
    }
  }

  return 0;
}

int thingy53_led_toggle(thingy53_led_color_t color) {
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

  return thingy53_led_toggle_impl(spec);
}
