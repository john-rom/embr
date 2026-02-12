#ifndef EMBR_APP_LOGIC_H_
#define EMBR_APP_LOGIC_H_

/**
 * @file embr_app_logic.h
 * @brief Pure state-transition logic for app runtime flow.
 *
 * This module is kernel-agnostic and side-effect free. It decides next-state
 * transitions and escalation outcomes; the runtime shell executes the actions.
 */

#include <stdint.h>

/** Runtime states in the app capture loop. */
enum embr_app_state {
  STATE_WAIT_WOS = 0,
  STATE_CAPTURE_SLICE,
  STATE_WINDOW_CHECK,
  STATE_RECOVER,
};

/** Transition result: next state plus error outcome. */
struct embr_app_transition {
  enum embr_app_state next_state;
  /** 0 for non-fatal progress, negative for fatal/escalated failure. */
  int err;
};

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Convert an app state enum to a printable string. */
const char *embr_app_state_to_str(enum embr_app_state state);

/**
 * @brief Decide next state after processing one capture-slice step.
 *
 * @param process_err Return code from embr_inference_process_wos_event_audio()
 * @param pdm_timeout_count In/out timeout counter
 * @param pdm_timeout_reboot_threshold Threshold before escalating fatal timeout
 *
 * @return Transition result with next state and error outcome
 */
struct embr_app_transition embr_app_next_state_from_capture_slice(
    int process_err, uint8_t *pdm_timeout_count,
    uint8_t pdm_timeout_reboot_threshold);

/**
 * @brief Decide next state after checking if inference window is full.
 *
 * @param window_check_ret Return code from embr_inference_stop_audio_if_window_full()
 *
 * @return Transition result with next state and error outcome
 */
struct embr_app_transition embr_app_next_state_from_window_check(
    int window_check_ret);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_APP_LOGIC_H_ */
