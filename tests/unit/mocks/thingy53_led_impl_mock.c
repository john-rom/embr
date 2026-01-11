#include "thingy53_led_impl_mock.h"
#include "thingy53_led_impl.h"

#include <stdbool.h>
#include <stddef.h>

const struct gpio_dt_spec red = {};
const struct gpio_dt_spec green = {};
const struct gpio_dt_spec blue = {};

const struct gpio_dt_spec *toggle_last_color = NULL;
int toggle_call_count = 0;
int toggle_return_value = 0;

int init_call_count = 0;
int init_return_value = 0;

void thingy53_led_impl_mock_reset(void) {
  toggle_last_color = NULL;
  toggle_call_count = 0;
  toggle_return_value = 0;
  init_call_count = 0;
  init_return_value = 0;
}

int thingy53_led_init_impl(const struct gpio_dt_spec *spec) {
  (void)spec;
  init_call_count++;
  return init_return_value;
}

int thingy53_led_toggle_impl(const struct gpio_dt_spec *color) {
  toggle_last_color = color;
  toggle_call_count++;
  return toggle_return_value;
}
