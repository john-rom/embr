#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/ztest.h>

#include "embr_app_logic.h"

ZTEST_SUITE(embr_app_logic, NULL, NULL, NULL, NULL, NULL);

ZTEST(embr_app_logic, test_state_to_str_known_states_success) {
  zassert_equal(strcmp(embr_app_state_to_str(STATE_WAIT_WOS), "WAIT_WOS"), 0,
                "STATE_WAIT_WOS should map to WAIT_WOS");
  zassert_equal(
      strcmp(embr_app_state_to_str(STATE_CAPTURE_SLICE), "CAPTURE_SLICE"), 0,
      "STATE_CAPTURE_SLICE should map to CAPTURE_SLICE");
  zassert_equal(
      strcmp(embr_app_state_to_str(STATE_WINDOW_CHECK), "WINDOW_CHECK"), 0,
      "STATE_WINDOW_CHECK should map to WINDOW_CHECK");
  zassert_equal(strcmp(embr_app_state_to_str(STATE_RECOVER), "RECOVER"), 0,
                "STATE_RECOVER should map to RECOVER");
}

ZTEST(embr_app_logic, test_state_to_str_invalid_state_returns_invalid_success) {
  zassert_equal(
      strcmp(embr_app_state_to_str((enum embr_app_state)99), "INVALID"), 0,
      "invalid state should map to INVALID");
}

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_zero_returns_window_check_success) {
  uint8_t pdm_timeout_count = 0;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(0, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_WINDOW_CHECK,
                "transition should move to STATE_WINDOW_CHECK");
  zassert_equal(transition.err, 0, "transition err should remain 0");
  zassert_equal(pdm_timeout_count, 0, "timeout count should remain unchanged");
}

ZTEST(
    embr_app_logic,
    test_next_state_from_capture_slice_timeout_below_threshold_returns_recover_success) {
  uint8_t pdm_timeout_count = 0;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(-ETIMEDOUT, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_RECOVER,
                "transition should move to STATE_RECOVER");
  zassert_equal(transition.err, 0, "transition err should remain 0");
  zassert_equal(pdm_timeout_count, 1, "timeout count should increment to 1");
}

ZTEST(
    embr_app_logic,
    test_next_state_from_capture_slice_timeout_at_threshold_returns_error_fail) {
  uint8_t pdm_timeout_count = 2;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(-ETIMEDOUT, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "transition should remain STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, -ETIMEDOUT,
                "transition err should be -ETIMEDOUT");
  zassert_equal(pdm_timeout_count, 3,
                "timeout count should increment at threshold");
}

ZTEST(embr_app_logic,
      test_next_state_from_capture_slice_other_error_returns_same_error_fail) {
  uint8_t pdm_timeout_count = 0;

  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(-EIO, &pdm_timeout_count, 3);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "transition should remain STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, -EIO, "transition err should propagate -EIO");
  zassert_equal(pdm_timeout_count, 0, "timeout count should remain unchanged");
}

ZTEST(
    embr_app_logic,
    test_next_state_from_capture_slice_null_timeout_count_returns_einval_fail) {
  struct embr_app_transition transition =
      embr_app_next_state_from_capture_slice(0, NULL, 3);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "transition should remain STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, -EINVAL, "transition err should be -EINVAL");
}

ZTEST(embr_app_logic,
      test_next_state_from_window_check_negative_returns_error_fail) {
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(-EIO);

  zassert_equal(transition.next_state, STATE_WINDOW_CHECK,
                "transition should remain STATE_WINDOW_CHECK");
  zassert_equal(transition.err, -EIO, "transition err should propagate -EIO");
}

ZTEST(embr_app_logic,
      test_next_state_from_window_check_zero_returns_capture_slice_success) {
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(0);

  zassert_equal(transition.next_state, STATE_CAPTURE_SLICE,
                "transition should move to STATE_CAPTURE_SLICE");
  zassert_equal(transition.err, 0, "transition err should remain 0");
}

ZTEST(embr_app_logic,
      test_next_state_from_window_check_positive_returns_wait_wos_success) {
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(1);

  zassert_equal(transition.next_state, STATE_WAIT_WOS,
                "transition should move to STATE_WAIT_WOS");
  zassert_equal(transition.err, 0, "transition err should remain 0");
}
