#include "thingy53_led_impl_mock.h"
#include "thingy53_led_impl.h"

thingy53_led_color_t mock_thingy53_led_toggle_last_color = NO_COLOR;
int mock_thingy53_led_toggle_call_count = 0;
int mock_thingy53_led_toggle_return_value = 0;
int mock_thingy53_led_init_call_count = 0;
int mock_thingy53_led_init_return_value = 0;

void thingy53_led_impl_mock_reset(void) {
  mock_thingy53_led_toggle_last_color = NO_COLOR;
  mock_thingy53_led_toggle_call_count = 0;
  mock_thingy53_led_toggle_return_value = 0;
  mock_thingy53_led_init_call_count = 0;
  mock_thingy53_led_init_return_value = 0;
}

int thingy53_led_init_impl(void) {
  mock_thingy53_led_init_call_count++;
  return mock_thingy53_led_init_return_value;
}

int thingy53_led_toggle_impl(thingy53_led_color_t color) {
  mock_thingy53_led_toggle_last_color = color;
  mock_thingy53_led_toggle_call_count++;
  return mock_thingy53_led_toggle_return_value;
}
