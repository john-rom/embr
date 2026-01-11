#ifndef THINGY53_LED_SPECS_H_
#define THINGY53_LED_SPECS_H_

/**
 * @file thingy53_led_specs.h
 * @brief Accessors for Thingy53 LED GPIO specs.
 *
 * Provides a devicetree-backed mapping from LED colors to GPIO specs.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_dt_spec;

/**
 * @brief Get the GPIO spec for the red LED.
 *
 * @return Pointer to the red LED GPIO spec.
 */
const struct gpio_dt_spec *thingy53_led_get_red(void);

/**
 * @brief Get the GPIO spec for the green LED.
 *
 * @return Pointer to the green LED GPIO spec.
 */
const struct gpio_dt_spec *thingy53_led_get_green(void);

/**
 * @brief Get the GPIO spec for the blue LED.
 *
 * @return Pointer to the blue LED GPIO spec.
 */
const struct gpio_dt_spec *thingy53_led_get_blue(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_LED_SPECS_H_ */
