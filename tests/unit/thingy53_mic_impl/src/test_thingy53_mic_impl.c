#include <errno.h>
#include <zephyr/ztest.h>

#include "dmic_wrap_mock.h"
#include "thingy53_mic_impl.h"
#include "thingy53_mic_specs_mock.h"

static void thingy53_mic_impl_before(void *fixture) {
  (void)fixture;
  dmic_wrap_mock_reset();
  thingy53_mic_specs_mock_reset();
}

ZTEST_SUITE(thingy53_mic_impl, NULL, NULL, thingy53_mic_impl_before, NULL,
            NULL);

ZTEST(thingy53_mic_impl, test_init_device_ready_success) {
  mock_thingy53_mic_device_ready = true;

  int ret = thingy53_mic_init_impl();
  zassert_equal(ret, 0, "thingy53_mic_init_impl should return 0");
  zassert_equal(mock_dmic_configure_call_count, 1,
                "thingy53_mic_init_impl should call dmic_wrap_configure once");
  zassert_equal(mock_dmic_trigger_call_count, 0,
                "thingy53_mic_init_impl should not call dmic_wrap_trigger");
}

ZTEST(thingy53_mic_impl, test_init_device_not_ready_fail) {
  mock_thingy53_mic_device_ready = false;

  int ret = thingy53_mic_init_impl();
  zassert_equal(ret, -ENODEV, "thingy53_mic_init_impl should return -ENODEV");
  zassert_equal(mock_dmic_configure_call_count, 0,
                "thingy53_mic_init_impl should not call dmic_wrap_configure");
  zassert_equal(mock_dmic_trigger_call_count, 0,
                "thingy53_mic_init_impl should not call dmic_wrap_trigger");
}

ZTEST(thingy53_mic_impl, test_init_error_on_config_fail) {
  mock_thingy53_mic_device_ready = true;
  mock_dmic_wrap_configure_ret = -EBUSY;

  int ret = thingy53_mic_init_impl();
  zassert_equal(ret, -EBUSY, "thingy53_mic_init_impl should return -EBUSY");
  zassert_equal(mock_dmic_configure_call_count, 1,
                "thingy53_mic_init_impl should call dmic_wrap_configure once");
  zassert_equal(mock_dmic_trigger_call_count, 0,
                "thingy53_mic_init_impl should not call dmic_wrap_trigger");
}

ZTEST(thingy53_mic_impl, test_start_trigger_success) {
  mock_dmic_wrap_trigger_ret = 0;

  int ret = thingy53_mic_start_impl();
  zassert_equal(ret, 0, "thingy53_mic_start_impl should return 0");
  zassert_equal(mock_dmic_trigger_call_count, 1,
                "thingy53_mic_start_impl should call dmic_wrap_trigger once");
  zassert_equal(mock_dmic_configure_call_count, 0,
                "thingy53_mic_start_impl should not call dmic_wrap_configure");
}

ZTEST(thingy53_mic_impl, test_start_error_on_trigger_fail) {
  mock_dmic_wrap_trigger_ret = -EINVAL;

  int ret = thingy53_mic_start_impl();
  zassert_equal(ret, -EINVAL, "thingy53_mic_start_impl should return -EINVAL");
  zassert_equal(mock_dmic_trigger_call_count, 1,
                "thingy53_mic_start_impl should call dmic_wrap_trigger once");
  zassert_equal(mock_dmic_configure_call_count, 0,
                "thingy53_mic_start_impl should not call dmic_wrap_configure");
}

ZTEST(thingy53_mic_impl, test_stop_trigger_success) {
  mock_dmic_wrap_trigger_ret = 0;

  int ret = thingy53_mic_stop_impl();
  zassert_equal(ret, 0, "thingy53_mic_stop_impl should return 0");
  zassert_equal(mock_dmic_trigger_call_count, 1,
                "thingy53_mic_stop_impl should call dmic_wrap_trigger once");
  zassert_equal(mock_dmic_configure_call_count, 0,
                "thingy53_mic_stop_impl should not call dmic_wrap_configure");
}

ZTEST(thingy53_mic_impl, test_stop_error_on_trigger_fail) {
  mock_dmic_wrap_trigger_ret = -EINVAL;

  int ret = thingy53_mic_stop_impl();
  zassert_equal(ret, -EINVAL, "thingy53_mic_stop_impl should return -EINVAL");
  zassert_equal(mock_dmic_trigger_call_count, 1,
                "thingy53_mic_stop_impl should call dmic_wrap_trigger once");
  zassert_equal(mock_dmic_configure_call_count, 0,
                "thingy53_mic_stop_impl should not call dmic_wrap_configure");
}
