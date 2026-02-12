#ifndef GPIO_WRAP_H_
#define GPIO_WRAP_H_

/**
 * @file gpio_wrap.h
 * @brief GPIO wrapper API for unit-test compatibility.
 *
 * Thin wrappers around Zephyr GPIO helpers, used to enable mocking in tests.
 */

#include <stdbool.h>
#include <stdint.h>

struct gpio_dt_spec;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Return true if the GPIO spec is ready.
 *
 * @param spec GPIO spec to check
 * @return true if ready; false otherwise
 */
bool gpio_wrap_is_ready_dt(const struct gpio_dt_spec *spec);

/**
 * @brief Configure a GPIO spec as output, inactive.
 *
 * @param spec GPIO spec to configure
 * @return 0 on success, or -errno on failure
 */
int gpio_wrap_pin_configure_dt_output_inactive(const struct gpio_dt_spec *spec);

/**
 * @brief Toggle a GPIO spec output.
 *
 * @param spec GPIO spec to toggle
 * @return 0 on success, or -errno on failure
 */
int gpio_wrap_pin_toggle_dt(const struct gpio_dt_spec *spec);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_WRAP_H_ */
