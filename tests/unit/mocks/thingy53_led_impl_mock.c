#include "thingy53_led_impl_mock.h"
#include "thingy53_led_impl.h"

thingy53_led_color_t toggle_last_color = NO_COLOR;
int toggle_call_count = 0;
int toggle_return_value = 0;
int init_call_count = 0;
int init_return_value = 0;

void thingy53_led_impl_mock_reset(void) {
  toggle_last_color = NO_COLOR;
  toggle_call_count = 0;
  toggle_return_value = 0;
  init_call_count = 0;
  init_return_value = 0;
}

int thingy53_led_init_impl(void) {
  init_call_count++;
  return init_return_value;
}

int thingy53_led_toggle_impl(thingy53_led_color_t color) {
  toggle_last_color = color;
  toggle_call_count++;
  return toggle_return_value;
}
