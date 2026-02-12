#include <errno.h>
#include <stdint.h>
#include <zephyr/ztest.h>

#include "embr_app_logic.h"

ZTEST_SUITE(embr_app_logic, NULL, NULL, NULL, NULL, NULL);

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_zero_returns_window_check_success) {
  uint8_t pdm_timeout_count = 0;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(0, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_WINDOW_CHECK,
                "next state should be STATE_WINDOW_CHECK");
  zassert_equal(transition.err, 0, "err should be 0");
  zassert_equal(pdm_timeout_count, 0,
                "pdm_timeout_count should remain unchanged");
}

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_timeout_below_threshold_returns_recover_success) {
  uint8_t pdm_timeout_count = 0;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(-ETIMEDOUT, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_RECOVER,
                "next state should be STATE_RECOVER");
  zassert_equal(transition.err, 0, "err should be 0");
  zassert_equal(pdm_timeout_count, 1,
                "pdm_timeout_count should increment to 1");
}

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_timeout_at_threshold_returns_error_fail) {
  uint8_t pdm_timeout_count = 2;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(-ETIMEDOUT, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "next state should remain STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, -ETIMEDOUT,
                "err should be -ETIMEDOUT at threshold");
  zassert_equal(pdm_timeout_count, 3,
                "pdm_timeout_count should increment at threshold");
}

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_other_error_returns_same_error_fail) {
  uint8_t pdm_timeout_count = 0;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(-EIO, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "next state should remain STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, -EIO, "err should propagate -EIO");
  zassert_equal(pdm_timeout_count, 0,
                "pdm_timeout_count should remain unchanged");
}

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_null_timeout_count_returns_einval_fail) {
  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(0, NULL, 3);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "next state should default to STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, -EINVAL, "err should be -EINVAL");
}

ZTEST(embr_app_logic,
      test_next_state_from_window_check_negative_returns_error_fail) {
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(-EIO);

  zassert_equal(transition.next_state, STATE_WINDOW_CHECK,
                "next state should remain STATE_WINDOW_CHECK");
  zassert_equal(transition.err, -EIO, "err should propagate -EIO");
}

ZTEST(embr_app_logic,
      test_next_state_from_window_check_zero_returns_capture_slice_success) {
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(0);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "next state should be STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, 0, "err should be 0");
}

ZTEST(embr_app_logic,
      test_next_state_from_window_check_positive_returns_wait_wos_success) {
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(1);

  zassert_equal(transition.next_state, STATE_WAIT_WOS,
                "next state should be STATE_WAIT_WOS");
  zassert_equal(transition.err, 0, "err should be 0");
}
