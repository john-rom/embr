#include <errno.h>
#include <zephyr/ztest.h>

#include "gpio_wrap_mock.h"
#include "thingy53_led.h"
#include "thingy53_led_impl.h"
#include "thingy53_led_specs.h"

static void thingy53_led_impl_before(void *fixture) {
  (void)fixture;
  gpio_wrap_mock_reset();
}

ZTEST_SUITE(thingy53_led_impl, NULL, NULL, thingy53_led_impl_before, NULL,
            NULL);

ZTEST(thingy53_led_impl, test_init_when_gpio_ready_success) {
  mock_gpio_wrap_is_ready_ret = true;
  mock_gpio_wrap_pin_configure_ret = 0;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, 0, "thingy53_led_init_impl should return 0");
}

ZTEST(thingy53_led_impl, test_init_when_gpio_not_ready_fail) {
  mock_gpio_wrap_is_ready_ret = false;
  mock_gpio_wrap_pin_configure_ret = 0;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, -ENODEV, "thingy53_led_init_impl should return -ENODEV");
}

ZTEST(thingy53_led_impl, test_init_on_configure_error_fail) {
  mock_gpio_wrap_is_ready_ret = true;
  mock_gpio_wrap_pin_configure_ret = -EIO;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, -EIO, "thingy53_led_init_impl should return -EIO");
}

ZTEST(thingy53_led_impl, test_init_when_gpio_not_ready_skips_configure_fail) {
  mock_gpio_wrap_is_ready_ret = false;
  mock_gpio_wrap_pin_configure_ret = -EIO;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, -ENODEV, "thingy53_led_init_impl should return -ENODEV");
}

ZTEST(thingy53_led_impl, test_toggle_valid_color_success) {
  const thingy53_led_color_t color = THINGY53_LED_BLUE;
  mock_gpio_wrap_pin_toggle_ret = 0;
  mock_gpio_wrap_pin_toggle_call_count = 0;

  int ret = thingy53_led_toggle_impl(color);
  zassert_equal(ret, 0, "thingy53_led_toggle_impl should return 0");
  zassert_equal(mock_gpio_wrap_pin_toggle_call_count, 1,
                "gpio_wrap_pin_toggle_dt should be called once");
  zassert_equal_ptr(mock_gpio_wrap_pin_toggle_last_spec,
                    thingy53_led_get_blue(),
                    "gpio_wrap_pin_toggle_dt should toggle blue spec");
}

ZTEST(thingy53_led_impl, test_toggle_invalid_color_fail) {
  const thingy53_led_color_t invalid_color = NO_COLOR;
  mock_gpio_wrap_pin_toggle_call_count = 0;

  int ret = thingy53_led_toggle_impl(invalid_color);
  zassert_equal(ret, -EINVAL, "thingy53_led_toggle_impl should return -EINVAL");
  zassert_equal(mock_gpio_wrap_pin_toggle_call_count, 0,
                "gpio_wrap_pin_toggle_dt should not be called");
}

ZTEST(thingy53_led_impl, test_toggle_on_gpio_toggle_error_fail) {
  const thingy53_led_color_t color = THINGY53_LED_RED;
  mock_gpio_wrap_pin_toggle_ret = -EIO;

  int ret = thingy53_led_toggle_impl(color);
  zassert_equal(ret, -EIO, "thingy53_led_toggle_impl should return -EIO");
  zassert_equal(mock_gpio_wrap_pin_toggle_call_count, 1,
                "gpio_wrap_pin_toggle_dt should be called once");
  zassert_equal_ptr(mock_gpio_wrap_pin_toggle_last_spec, thingy53_led_get_red(),
                    "gpio_wrap_pin_toggle_dt should toggle red spec");
}
