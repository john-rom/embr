#include "thingy53_led_impl.h"

#include <errno.h>

#include <zephyr/sys/__assert.h>

#include "gpio_wrap.h"

int thingy53_led_init_impl(const struct gpio_dt_spec *spec) {
  __ASSERT(spec != NULL, "gpio spec is null");
  if (!gpio_wrap_is_ready_dt(spec)) {
    return -ENODEV;
  }

  int err = gpio_wrap_pin_configure_dt_output_inactive(spec);
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_led_toggle_impl(const struct gpio_dt_spec *color) {
  __ASSERT(color != NULL, "gpio spec is null");
  int err = gpio_wrap_pin_toggle_dt(color);
  if (err) {
    return err;
  }

  return 0;
}
