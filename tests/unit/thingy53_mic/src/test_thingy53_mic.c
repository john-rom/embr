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
  mock_thingy53_mic_init_impl_return_value = 0;
  int ret = thingy53_mic_init();
  zassert_equal(ret, 0, "thingy53_mic_init should return 0");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 1,
                "thingy53_mic_init_impl should be called once");
  zassert_equal(mock_thingy53_mic_start_impl_call_count, 0,
                "thingy53_mic_start_impl should not be called");
}

ZTEST(thingy53_mic, test_init_returns_error_on_fail) {
  mock_thingy53_mic_init_impl_return_value = -ENODEV;
  int ret = thingy53_mic_init();
  zassert_equal(ret, -ENODEV, "thingy53_mic_init should return -ENODEV");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 1,
                "thingy53_mic_init_impl should be called once");
  zassert_equal(mock_thingy53_mic_start_impl_call_count, 0,
                "thingy53_mic_start_impl should not be called");
}

ZTEST(thingy53_mic, test_start_returns_zero_on_success) {
  mock_thingy53_mic_start_impl_return_value = 0;
  int ret = thingy53_mic_start();
  zassert_equal(ret, 0, "thingy53_mic_start should return 0");
  zassert_equal(mock_thingy53_mic_start_impl_call_count, 1,
                "thingy53_mic_start should be called once");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 0,
                "thingy53_mic_init_impl should not be called");
}

ZTEST(thingy53_mic, test_start_returns_error_on_fail) {
  mock_thingy53_mic_start_impl_return_value = -EINVAL;
  int ret = thingy53_mic_start();
  zassert_equal(ret, -EINVAL, "thingy53_mic_start should return -EINVAL");
  zassert_equal(mock_thingy53_mic_start_impl_call_count, 1,
                "thingy53_mic_start should be called once");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 0,
                "thingy53_mic_init_impl should not be called");
}

ZTEST(thingy53_mic, test_stop_returns_zero_on_success) {
  mock_thingy53_mic_stop_impl_return_value = 0;
  int ret = thingy53_mic_stop();
  zassert_equal(ret, 0, "thingy53_mic_stop should return 0");
  zassert_equal(mock_thingy53_mic_stop_impl_call_count, 1,
                "thingy53_mic_stop should be called once");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 0,
                "thingy53_mic_init_impl should not be called");
}

ZTEST(thingy53_mic, test_stop_returns_error_on_fail) {
  mock_thingy53_mic_stop_impl_return_value = -EINVAL;
  int ret = thingy53_mic_stop();
  zassert_equal(ret, -EINVAL, "thingy53_mic_stop should return -EINVAL");
  zassert_equal(mock_thingy53_mic_stop_impl_call_count, 1,
                "thingy53_mic_stop should be called once");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 0,
                "thingy53_mic_init_impl should not be called");
}

ZTEST(thingy53_mic, test_reset_returns_zero_on_success) {
  mock_thingy53_mic_reset_impl_return_value = 0;
  int ret = thingy53_mic_reset();
  zassert_equal(ret, 0, "thingy53_mic_reset should return 0");
  zassert_equal(mock_thingy53_mic_reset_impl_call_count, 1,
                "thingy53_mic_reset should be called once");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 0,
                "thingy53_mic_init_impl should not be called");
}

ZTEST(thingy53_mic, test_reset_returns_error_on_fail) {
  mock_thingy53_mic_reset_impl_return_value = -EIO;
  int ret = thingy53_mic_reset();
  zassert_equal(ret, -EIO, "thingy53_mic_reset should return -EIO");
  zassert_equal(mock_thingy53_mic_reset_impl_call_count, 1,
                "thingy53_mic_reset should be called once");
  zassert_equal(mock_thingy53_mic_init_impl_call_count, 0,
                "thingy53_mic_init_impl should not be called");
}

ZTEST(thingy53_mic, test_read_returns_zero_on_success) {
  void **buffer = NULL;
  size_t *size = 0;
  int32_t timeout = 0;
  mock_thingy53_mic_read_impl_return_value = 0;

  int ret = thingy53_mic_read(buffer, size, timeout);
  zassert_equal(ret, 0, "thingy53_mic_read should return 0");
  zassert_equal(mock_thingy53_mic_read_impl_call_count, 1,
                "thingy53_mic_read should be called once");
}

ZTEST(thingy53_mic, test_read_returns_error_on_fail) {
  void **buffer = NULL;
  size_t *size = 0;
  int32_t timeout = 0;
  mock_thingy53_mic_read_impl_return_value = -ENODEV;

  int ret = thingy53_mic_read(buffer, size, timeout);
  zassert_equal(ret, -ENODEV, "thingy53_mic_read should return -ENODEV");
  zassert_equal(mock_thingy53_mic_read_impl_call_count, 1,
                "thingy53_mic_read should be called once");
}
