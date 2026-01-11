#include "thingy53_led_impl_mock.h"
#include "thingy53_led_specs.h"

const struct gpio_dt_spec *thingy53_led_get_red(void) { return &red; }

const struct gpio_dt_spec *thingy53_led_get_green(void) { return &green; }

const struct gpio_dt_spec *thingy53_led_get_blue(void) { return &blue; }
