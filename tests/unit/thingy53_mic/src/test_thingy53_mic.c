#include <errno.h>
#include <zephyr/ztest.h>

#include "thingy53_mic.h"
#include "thingy53_mic_impl_mock.h"

static void thingy53_mic_before(void *fixture) {
  (void)fixture;
  thingy53_mic_impl_mock_reset();
}

ZTEST_SUITE(thingy53_mic, NULL, NULL, thingy53_mic_before, NULL, NULL);

ZTEST(thingy53_mic, test_init_returns_zero_on_success) {
  init_return_value = 0;
  int ret = thingy53_mic_init();
  zassert_equal(ret, 0, "thingy53_mic_init should return 0");
  zassert_equal(init_call_count, 1, "thingy53_mic_init should be called once");
}

ZTEST(thingy53_mic, test_init_returns_error_on_fail) {
  init_return_value = -ENODEV;
  int ret = thingy53_mic_init();
  zassert_equal(ret, -ENODEV, "thingy53_mic_init should return -ENODEV");
}
