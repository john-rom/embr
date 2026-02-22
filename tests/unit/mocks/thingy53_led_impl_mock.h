#ifndef THINGY53_LED_IMPL_MOCK_H_
#define THINGY53_LED_IMPL_MOCK_H_

/**
 * @file thingy53_led_impl_mock.h
 * @brief Mock helpers for Thingy53 LED implementation.
 */

#include "thingy53_led.h"

#ifdef __cplusplus
extern "C" {
#endif

extern thingy53_led_color_t mock_thingy53_led_toggle_impl_last_color;
extern int mock_thingy53_led_toggle_impl_call_count;
extern int mock_thingy53_led_toggle_impl_return_value;
extern int mock_thingy53_led_init_impl_call_count;
extern int mock_thingy53_led_init_impl_return_value;

void thingy53_led_impl_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_LED_IMPL_MOCK_H_ */
