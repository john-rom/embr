#include <errno.h>
#include <zephyr/ztest.h>

#include "thingy53_led.h"
#include "thingy53_led_impl_mock.h"

static void thingy53_led_before(void *fixture) {
  (void)fixture;
  thingy53_led_impl_mock_reset();
}

ZTEST_SUITE(thingy53_led, NULL, NULL, thingy53_led_before, NULL, NULL);

ZTEST(thingy53_led, test_init_success) {
  mock_thingy53_led_init_impl_return_value = 0;

  int ret = thingy53_led_init();
  zassert_equal(ret, 0, "thingy53_led_init should return 0");
}

ZTEST(thingy53_led, test_init_fail) {
  mock_thingy53_led_init_impl_return_value = -EIO;

  int ret = thingy53_led_init();
  zassert_equal(ret, -EIO, "thingy53_led_init should return -EIO");
}

ZTEST(thingy53_led, test_toggle_valid_color_success) {
  const thingy53_led_color_t color = THINGY53_LED_GREEN;
  mock_thingy53_led_toggle_impl_return_value = 0;

  int ret = thingy53_led_toggle(color);
  zassert_equal(ret, 0, "thingy53_led_toggle should return 0");
  zassert_equal(mock_thingy53_led_toggle_impl_call_count, 1,
                "thingy53_led_toggle_impl should be called once");
  zassert_equal(mock_thingy53_led_toggle_impl_last_color, THINGY53_LED_GREEN,
                "thingy53_led_toggle_impl should toggle green LED");
}

ZTEST(thingy53_led, test_toggle_invalid_color_fail) {
  const thingy53_led_color_t invalid_color = NO_COLOR;
  mock_thingy53_led_toggle_impl_return_value = -EIO;

  int ret = thingy53_led_toggle(invalid_color);
  zassert_equal(ret, -EIO, "thingy53_led_toggle should return -EIO");
  zassert_equal(mock_thingy53_led_toggle_impl_call_count, 1,
                "thingy53_led_toggle_impl should be called once");
  zassert_equal(
      mock_thingy53_led_toggle_impl_last_color, NO_COLOR,
      "thingy53_led_toggle_impl should attempt to toggle invalid color");
}
