#include "gpio_wrap_mock.h"
#include "thingy53_led_specs.h"

static const struct gpio_dt_spec mock_spec;
const struct gpio_dt_spec *mock_thingy53_led_red_spec = &mock_spec;
const struct gpio_dt_spec *mock_thingy53_led_green_spec = &mock_spec;
const struct gpio_dt_spec *mock_thingy53_led_blue_spec = &mock_spec;

const struct gpio_dt_spec *thingy53_led_get_red(void) {
  return mock_thingy53_led_red_spec;
}

const struct gpio_dt_spec *thingy53_led_get_green(void) {
  return mock_thingy53_led_green_spec;
}

const struct gpio_dt_spec *thingy53_led_get_blue(void) {
  return mock_thingy53_led_blue_spec;
}
