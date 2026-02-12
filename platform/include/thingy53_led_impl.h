#ifndef THINGY53_LED_IMPL_H_
#define THINGY53_LED_IMPL_H_

/**
 * @file thingy53_led_impl.h
 * @brief Thingy53 LED implementation helpers for specific GPIO specs.
 */

#include "thingy53_led.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Thingy53 LEDs.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_led_init_impl(void);

/**
 * @brief Toggle a specific Thingy53 LED.
 *
 * @param color LED color to toggle
 * @return 0 on success, or -errno on failure
 */
int thingy53_led_toggle_impl(thingy53_led_color_t color);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_LED_IMPL_H_ */
