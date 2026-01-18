#include <errno.h>
#include <zephyr/ztest.h>

#include "thingy53_mic_impl.h"
#include "thingy53_mic_specs_mock.h"

static void thingy53_mic_impl_before(void *fixture) {
  (void)fixture;
  thingy53_mic_specs_mock_reset();
}

ZTEST_SUITE(thingy53_mic_impl, NULL, NULL, thingy53_mic_impl_before, NULL,
            NULL);

ZTEST(thingy53_mic_impl, test_init_device_ready_success) {
  mock_thingy53_mic_device_ready = true;
  int ret = thingy53_mic_init_impl();
  zassert_equal(ret, 0, "thingy53_mic_init_impl should return 0");
}

ZTEST(thingy53_mic_impl, test_init_device_not_ready_fail) {
  mock_thingy53_mic_device_ready = false;
  int ret = thingy53_mic_init_impl();
  zassert_equal(ret, -ENODEV, "thingy53_mic_init_impl should return -ENODEV");
}
