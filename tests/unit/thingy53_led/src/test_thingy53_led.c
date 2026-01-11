#include <errno.h>
#include <zephyr/ztest.h>

#include "thingy53_led.h"
#include "thingy53_led_impl_mock.h"

static void thingy53_led_before(void *fixture) {
  (void)fixture;
  thingy53_led_impl_mock_reset();
}

ZTEST_SUITE(thingy53_led, NULL, NULL, thingy53_led_before, NULL, NULL);

ZTEST(thingy53_led, test_init_calls_impl_for_each_led_success) {
  init_return_value = 0;

  int ret = thingy53_led_init();
  zassert_equal(ret, 0, "thingy53_led_init should return 0");
  zassert_equal(init_call_count, 3, "init should be called for each LED");
}

ZTEST(thingy53_led, test_init_propagates_impl_error_fail) {
  init_return_value = -EIO;

  int ret = thingy53_led_init();
  zassert_equal(ret, -EIO, "thingy53_led_init should return -EIO");
}

ZTEST(thingy53_led, test_toggle_calls_impl_with_color_success) {
  toggle_return_value = 0;

  int ret = thingy53_led_toggle(THINGY53_LED_GREEN);
  zassert_equal(ret, 0, "thingy53_led_toggle should return 0");
  zassert_equal(toggle_call_count, 1, "toggle should be called once");
  zassert_equal(toggle_last_color, &green, "toggle should use green GPIO");
}

ZTEST(thingy53_led, test_toggle_invalid_color_returns_einval_fail) {
  int ret = thingy53_led_toggle((thingy53_led_color_t)99);
  zassert_equal(ret, -EINVAL, "invalid color should return -EINVAL");
  zassert_equal(toggle_call_count, 0, "toggle should not be called");
}
