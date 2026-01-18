#include "thingy53_led.h"
#include "thingy53_led_impl.h"

int thingy53_led_init(void) {
  int err = thingy53_led_init_impl();
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_led_toggle(thingy53_led_color_t color) {
  return thingy53_led_toggle_impl(color);
}
