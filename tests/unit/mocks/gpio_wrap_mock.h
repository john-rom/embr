#ifndef GPIO_WRAP_MOCK_H_
#define GPIO_WRAP_MOCK_H_

/**
 * @file gpio_wrap_mock.h
 * @brief Mock helpers for GPIO wrapper API.
 */

#include <stdbool.h>
#include <stdint.h>

struct gpio_dt_spec {
  uint8_t dummy;
};

#ifdef __cplusplus
extern "C" {
#endif

extern bool mock_gpio_wrap_is_ready_ret;
extern int mock_gpio_wrap_pin_configure_ret;
extern int mock_gpio_wrap_pin_toggle_ret;

void gpio_wrap_mock_reset(void);

bool gpio_wrap_is_ready_dt(const struct gpio_dt_spec *spec);
int gpio_wrap_pin_configure_dt_output_inactive(const struct gpio_dt_spec *spec);
int gpio_wrap_pin_toggle_dt(const struct gpio_dt_spec *spec);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_WRAP_MOCK_H_ */
