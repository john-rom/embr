#ifndef THINGY53_LED_H_
#define THINGY53_LED_H_

/**
 * @file thingy53_led.h
 * @brief Thingy53 LED control API.
 *
 * High-level interface for initializing and toggling Thingy53 LEDs.
 * All functions return 0 on success or -errno on failure.
 */

/** LED color selector. */
typedef enum {
  NO_COLOR = 0,
  THINGY53_LED_RED,
  THINGY53_LED_GREEN,
  THINGY53_LED_BLUE,
} thingy53_led_color_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize all Thingy53 LEDs.
 *
 * @return 0 on success, or -errno on failure
 */
int thingy53_led_init(void);

/**
 * @brief Toggle a Thingy53 LED.
 *
 * @param color LED color to toggle
 * @return 0 on success, or -errno on failure
 */
int thingy53_led_toggle(thingy53_led_color_t color);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_LED_H_ */
