#include "gpio_wrap_mock.h"
#include "thingy53_led_specs.h"

const struct gpio_dt_spec *mock_thingy53_led_get_red;
const struct gpio_dt_spec *mock_thingy53_led_get_green;
const struct gpio_dt_spec *mock_thingy53_led_get_blue;

const struct gpio_dt_spec *thingy53_led_get_red(void) {
  return mock_thingy53_led_get_red;
}

const struct gpio_dt_spec *thingy53_led_get_green(void) {
  return mock_thingy53_led_get_green;
}

const struct gpio_dt_spec *thingy53_led_get_blue(void) {
  return mock_thingy53_led_get_blue;
}
