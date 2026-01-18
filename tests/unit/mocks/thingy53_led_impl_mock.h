#ifndef THINGY53_LED_IMPL_MOCK_H_
#define THINGY53_LED_IMPL_MOCK_H_

#include "thingy53_led.h"

#ifdef __cplusplus
extern "C" {
#endif

extern thingy53_led_color_t toggle_last_color;
extern int toggle_call_count;
extern int toggle_return_value;
extern int init_call_count;
extern int init_return_value;

void thingy53_led_impl_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_LED_IMPL_MOCK_H_ */
