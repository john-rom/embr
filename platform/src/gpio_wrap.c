#include "gpio_wrap.h"

#include <zephyr/drivers/gpio.h>

bool gpio_wrap_is_ready_dt(const struct gpio_dt_spec *spec) {
  return gpio_is_ready_dt(spec);
}

int gpio_wrap_pin_configure_dt_output_inactive(
    const struct gpio_dt_spec *spec) {
  return gpio_pin_configure_dt(spec, GPIO_OUTPUT_INACTIVE);
}

int gpio_wrap_pin_toggle_dt(const struct gpio_dt_spec *spec) {
  return gpio_pin_toggle_dt(spec);
}
