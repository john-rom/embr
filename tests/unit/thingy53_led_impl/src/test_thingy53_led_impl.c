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

ZTEST(thingy53_led_impl, test_init_impl_gpio_ready_and_config_success) {
  mock_gpio_wrap_is_ready_ret = true;
  mock_gpio_wrap_pin_configure_ret = 0;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, 0, "thingy53_led_init_impl should return 0");
}

ZTEST(thingy53_led_impl, test_init_impl_gpio_not_ready_fail) {
  mock_gpio_wrap_is_ready_ret = false;
  mock_gpio_wrap_pin_configure_ret = 0;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, -ENODEV, "thingy53_led_init_impl should return -ENODEV");
}

ZTEST(thingy53_led_impl, test_init_impl_gpio_config_fail) {
  mock_gpio_wrap_is_ready_ret = true;
  mock_gpio_wrap_pin_configure_ret = -EIO;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, -EIO, "thingy53_led_init_impl should return -EIO");
}

ZTEST(thingy53_led_impl, test_init_impl_gpio_not_ready_not_config_fail) {
  mock_gpio_wrap_is_ready_ret = false;
  mock_gpio_wrap_pin_configure_ret = -EIO;

  int ret = thingy53_led_init_impl();
  zassert_equal(ret, -ENODEV, "thingy53_led_init_impl should return -ENODEV");
}

ZTEST(thingy53_led_impl, test_toggle_gpio_pin_toggle_success) {
  thingy53_led_color_t color = THINGY53_LED_RED;
  mock_gpio_wrap_pin_toggle_ret = 0;

  int ret = thingy53_led_toggle_impl(color);
  zassert_equal(ret, 0, "thingy53_led_toggle_impl should return 0");
}

ZTEST(thingy53_led_impl, test_toggle_gpio_pin_toggle_fail) {
  thingy53_led_color_t color = THINGY53_LED_RED;
  mock_gpio_wrap_pin_toggle_ret = -EIO;

  int ret = thingy53_led_toggle_impl(color);
  zassert_equal(ret, -EIO, "thingy53_led_toggle_impl should return -EIO");
}
