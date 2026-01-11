#ifndef GPIO_WRAP_MOCK_H_
#define GPIO_WRAP_MOCK_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_dt_spec {
  uint8_t dummy;
};

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
