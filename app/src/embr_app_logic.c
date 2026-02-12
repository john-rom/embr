#include "embr_app_logic.h"

#include <errno.h>
#include <stddef.h>
#include <zephyr/sys/__assert.h>

const char *embr_app_state_to_str(enum embr_app_state state) {
  switch (state) {
  case STATE_WAIT_WOS:
    return "WAIT_WOS";
  case STATE_CAPTURE_SLICE:
    return "CAPTURE_SLICE";
  case STATE_WINDOW_CHECK:
    return "WINDOW_CHECK";
  case STATE_RECOVER:
    return "RECOVER";
  default:
    return "INVALID";
  }
}

struct embr_app_transition embr_app_next_state_from_capture_slice(
    int process_err, uint8_t *pdm_timeout_count,
    uint8_t pdm_timeout_reboot_threshold) {
  struct embr_app_transition transition = {
      .next_state = STATE_CAPTURE_SLICE,
      .err = 0,
  };

  __ASSERT(pdm_timeout_count != NULL, "pdm_timeout_count is null");
  if (pdm_timeout_count == NULL) {
    transition.err = -EINVAL;
    return transition;
  }

  if (process_err == 0) {
    transition.next_state = STATE_WINDOW_CHECK;
    return transition;
  }

  if (process_err == -ETIMEDOUT) {
    (*pdm_timeout_count)++;
    if (*pdm_timeout_count >= pdm_timeout_reboot_threshold) {
      transition.err = -ETIMEDOUT;
      return transition;
    }
    transition.next_state = STATE_RECOVER;
    return transition;
  }

  transition.err = process_err;
  return transition;
}

struct embr_app_transition embr_app_next_state_from_window_check(
    int window_check_ret) {
  struct embr_app_transition transition = {
      .next_state = STATE_CAPTURE_SLICE,
      .err = 0,
  };

  if (window_check_ret < 0) {
    transition.next_state = STATE_WINDOW_CHECK;
    transition.err = window_check_ret;
    return transition;
  }
  if (window_check_ret > 0) {
    transition.next_state = STATE_WAIT_WOS;
    return transition;
  }
  return transition;
}
