#ifndef THINGY53_LED_IMPL_H_
#define THINGY53_LED_IMPL_H_

/**
 * @file thingy53_led_impl.h
 * @brief Thingy53 LED implementation helpers for specific GPIO specs.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_dt_spec;

/**
 * @brief Initialize a single LED GPIO spec.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_led_init_impl(const struct gpio_dt_spec *spec);

/**
 * @brief Toggle a LED for the provided GPIO spec.
 *
 * @return 0 on success, or -errno on failure.
 */
int thingy53_led_toggle_impl(const struct gpio_dt_spec *color);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_LED_IMPL_H_ */
