#include "gpio_wrap_mock.h"

bool mock_gpio_wrap_is_ready_ret = false;
int mock_gpio_wrap_pin_configure_ret = 0;
int mock_gpio_wrap_pin_toggle_ret = 0;

void gpio_wrap_mock_reset(void) {
  mock_gpio_wrap_is_ready_ret = false;
  mock_gpio_wrap_pin_configure_ret = 0;
  mock_gpio_wrap_pin_toggle_ret = 0;
}

bool gpio_wrap_is_ready_dt(const struct gpio_dt_spec *spec) {
  (void)spec;
  return mock_gpio_wrap_is_ready_ret;
}

int gpio_wrap_pin_configure_dt_output_inactive(
    const struct gpio_dt_spec *spec) {
  (void)spec;
  return mock_gpio_wrap_pin_configure_ret;
}

int gpio_wrap_pin_toggle_dt(const struct gpio_dt_spec *spec) {
  (void)spec;
  return mock_gpio_wrap_pin_toggle_ret;
}
