#include <errno.h>
#include <zephyr/ztest.h>

#include "thingy53_mic.h"
#include "thingy53_mic_impl_mock.h"

static void thingy53_mic_before(void *fixture) {
  (void)fixture;
  thingy53_mic_impl_mock_reset();
}

ZTEST_SUITE(thingy53_mic, NULL, NULL, thingy53_mic_before, NULL, NULL);

ZTEST(thingy53_mic, test_init_success) {
  mock_thingy53_mic_init_impl_return_value = 0;

  int ret = thingy53_mic_init();
  zassert_equal(ret, 0, "thingy53_mic_init should return 0");
}

ZTEST(thingy53_mic, test_init_fail) {
  mock_thingy53_mic_init_impl_return_value = -EIO;

  int ret = thingy53_mic_init();
  zassert_equal(ret, -EIO, "thingy53_mic_init should return -EIO");
}

ZTEST(thingy53_mic, test_start_success) {
  mock_thingy53_mic_start_impl_return_value = 0;

  int ret = thingy53_mic_start();
  zassert_equal(ret, 0, "thingy53_mic_start should return 0");
}

ZTEST(thingy53_mic, test_start_fail) {
  mock_thingy53_mic_start_impl_return_value = -EIO;

  int ret = thingy53_mic_start();
  zassert_equal(ret, -EIO, "thingy53_mic_start should return -EIO");
}

ZTEST(thingy53_mic, test_stop_success) {
  mock_thingy53_mic_stop_impl_return_value = 0;

  int ret = thingy53_mic_stop();
  zassert_equal(ret, 0, "thingy53_mic_stop should return 0");
}

ZTEST(thingy53_mic, test_stop_fail) {
  mock_thingy53_mic_stop_impl_return_value = -EIO;

  int ret = thingy53_mic_stop();
  zassert_equal(ret, -EIO, "thingy53_mic_stop should return -EIO");
}

ZTEST(thingy53_mic, test_deinit_success) {
  mock_thingy53_mic_deinit_impl_return_value = 0;

  int ret = thingy53_mic_deinit();
  zassert_equal(ret, 0, "thingy53_mic_deinit should return 0");
}

ZTEST(thingy53_mic, test_deinit_fail) {
  mock_thingy53_mic_deinit_impl_return_value = -EIO;

  int ret = thingy53_mic_deinit();
  zassert_equal(ret, -EIO, "thingy53_mic_deinit should return -EIO");
}

ZTEST(thingy53_mic, test_reset_success) {
  mock_thingy53_mic_reset_impl_return_value = 0;

  int ret = thingy53_mic_reset();
  zassert_equal(ret, 0, "thingy53_mic_reset should return 0");
}

ZTEST(thingy53_mic, test_reset_fail) {
  mock_thingy53_mic_reset_impl_return_value = -EIO;

  int ret = thingy53_mic_reset();
  zassert_equal(ret, -EIO, "thingy53_mic_reset should return -EIO");
}

ZTEST(thingy53_mic, test_read_success) {
  void **buffer = NULL;
  size_t *size = 0;
  int32_t timeout = 0;
  mock_thingy53_mic_read_impl_return_value = 0;

  int ret = thingy53_mic_read(buffer, size, timeout);
  zassert_equal(ret, 0, "thingy53_mic_read should return 0");
}

ZTEST(thingy53_mic, test_read_fail) {
  void **buffer = NULL;
  size_t *size = 0;
  int32_t timeout = 0;
  mock_thingy53_mic_read_impl_return_value = -EIO;

  int ret = thingy53_mic_read(buffer, size, timeout);
  zassert_equal(ret, -EIO, "thingy53_mic_read should return -EIO");
}
