#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

#include "ei_wrap.h"
#include "embr_app.h"
#include "embr_app_logic.h"
#include "embr_classify.h"
#include "embr_error_id.h"
#include "embr_inference.h"
#include "embr_types.h"
#include "thingy53_led.h"
#include "thingy53_mic.h"
#include "vm3011.h"

LOG_MODULE_REGISTER(embr_app);

#define EMBR_PDM_TIMEOUT_REBOOT_THRESHOLD 3

struct led_work {
  struct k_work off_work;
  thingy53_led_color_t color;
};

static struct led_work led_work_toggle;

static void led_off_work_handler(struct k_work *work) {
  struct led_work *lw = CONTAINER_OF(work, struct led_work, off_work);
  int err = thingy53_led_toggle(lw->color);
  if (err) {
    embr_error_report(EMBR_ERR_ID_LED_TOGGLE);
  }
}

int embr_app_led_toggle_on_capture_start(void) {
  led_work_toggle.color = THINGY53_LED_GREEN;
  return thingy53_led_toggle(led_work_toggle.color);
}

int embr_app_led_toggle_on_capture_end_async(void) {
  k_work_submit(&led_work_toggle.off_work);
  return 0;
}

K_SEM_DEFINE(wos_sem, 0, 1);
K_SEM_DEFINE(pdm_sem, 0, 1);

struct k_sem *embr_app_wos_sem(void) { return &wos_sem; }
struct k_sem *embr_app_pdm_sem(void) { return &pdm_sem; }

void vm3011_wos_triggered_hook(void) { k_sem_give(&wos_sem); }
void vm3011_buffer_released_hook(void) { k_sem_give(&pdm_sem); }
void vm3011_error_hook(vm3011_error_t err) {
  switch (err) {
  case VM3011_ERR_PDM_OVERFLOW:
    embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
    break;
  case VM3011_ERR_PDM_SET_BUFFER:
    embr_error_report(EMBR_ERR_ID_PDM_SET_BUFFER);
    break;
  default:
    break;
  }
}

static void embr_app_error_handler(embr_error_id_t id) {
  LOG_ERR("%s", embr_error_id_to_str(id));
}

static bool app_initialized = false;

static embr_command command = EMBR_RESET;
static embr_classification_results results;
static embr_label_scores scores;

struct embr_app_runtime {
  enum embr_app_state state;
  uint8_t pdm_timeout_count;
};

static void embr_app_set_state(enum embr_app_state *state,
                               enum embr_app_state next) {
  if (*state != next) {
    LOG_INF("State %s -> %s", embr_app_state_to_str(*state),
            embr_app_state_to_str(next));
    *state = next;
  }
}

static int embr_app_handle_state_wait_wos(struct embr_app_runtime *runtime) {
  int err = embr_inference_begin_audio_capture_after_wos();
  if (err) {
    return err;
  }
  embr_app_set_state(&runtime->state, STATE_CAPTURE_SLICE);
  return 0;
}

static int embr_app_handle_state_capture_slice(struct embr_app_runtime *runtime) {
  int err = embr_inference_process_wos_event_audio();
  struct embr_app_transition transition = embr_app_next_state_from_capture_slice(
      err, &runtime->pdm_timeout_count, EMBR_PDM_TIMEOUT_REBOOT_THRESHOLD);

  if (err == -ETIMEDOUT && transition.err == 0) {
    LOG_WRN("PDM timeout (%u). Recovering mic...",
            (unsigned int)runtime->pdm_timeout_count);
  }
  if (transition.err) {
    if (transition.err == -ETIMEDOUT) {
      LOG_ERR("PDM timeout threshold reached (%u). Escalating...",
              (unsigned int)EMBR_PDM_TIMEOUT_REBOOT_THRESHOLD);
    }
    return transition.err;
  }

  embr_app_set_state(&runtime->state, transition.next_state);
  return 0;
}

static int embr_app_handle_state_window_check(struct embr_app_runtime *runtime) {
  int window_check_ret = embr_inference_stop_audio_if_window_full();
  struct embr_app_transition transition =
      embr_app_next_state_from_window_check(window_check_ret);
  if (transition.err) {
    return transition.err;
  }

  embr_app_set_state(&runtime->state, transition.next_state);
  return 0;
}

static int embr_app_handle_state_recover(struct embr_app_runtime *runtime) {
  int err = thingy53_mic_stop();
  if (err) {
    LOG_WRN("Thingy53 mic stop failed during recover: %d", err);
  }

  err = thingy53_mic_deinit();
  if (err) {
    LOG_WRN("Thingy53 mic deinit failed during recover: %d", err);
  }

  err = thingy53_mic_init();
  if (err) {
    LOG_ERR("Thingy53 mic reinit failed: %d", err);
    return err;
  }

  embr_inference_reset_state();
  embr_app_set_state(&runtime->state, STATE_WAIT_WOS);
  return 0;
}

static void result_ready_cb(int err) {
  if (err) {
    LOG_ERR("Inference callback error: %d", err);
    goto restart_prediction;
  }

  err = embr_classify_get_all_results(&results, &scores);
  if (err) {
    LOG_ERR("Failed to get inference results: %d", err);
    goto restart_prediction;
  }

  if (ei_wrap_classifier_has_anomaly()) {
    err = ei_wrap_get_anomaly(&results.anomaly);
    if (err) {
      LOG_WRN("Failed to get anomaly result: %d", err);
    } else {
      LOG_INF("Anomaly: %.2f", (double)results.anomaly);
    }
  }

  err = embr_classify_set_command(scores, &command);
  if (err) {
    LOG_ERR("Failed to set embr command: %d", err);
    goto restart_prediction;
  }
  LOG_INF("Inference result: %s", embr_classify_command_to_str(command));

restart_prediction:
  command = EMBR_RESET;

  err = ei_wrap_start_prediction(1, 0);
  if (err) {
    LOG_ERR("Failed to restart prediction: %d", err);
    return;
  }
  LOG_INF("Prediction restarted...");
}

int embr_app_init(void) {
  int err = 0;

  err = ei_wrap_init(result_ready_cb);
  if (err) {
    LOG_ERR("Edge Impulse wrapper init failed: %d", err);
    return err;
  };
  LOG_INF("Edge Impulse wrapper is ready");

  err = thingy53_led_init();
  if (err) {
    LOG_ERR("Thingy53 LED init failed: %d", err);
    return err;
  }
  LOG_INF("Thingy53 LED is ready");

  err = thingy53_mic_init();
  if (err) {
    LOG_ERR("Thingy53 mic init failed: %d", err);
    return err;
  }
  LOG_INF("Thingy53 mic is ready");

  embr_error_register_handler(embr_app_error_handler);

  k_work_init(&led_work_toggle.off_work, led_off_work_handler);

  app_initialized = true;

  return 0;
}

int embr_app_start(void) {
  __ASSERT(app_initialized, "embr_app_start called before embr_app_init");
  if (!app_initialized) {
    return -EINVAL;
  }

  int err = embr_inference_start();
  if (err) {
    return err;
  }

  struct embr_app_runtime runtime = {
      .state = STATE_WAIT_WOS,
      .pdm_timeout_count = 0,
  };

  while (1) {
    switch (runtime.state) {
    case STATE_WAIT_WOS:
      err = embr_app_handle_state_wait_wos(&runtime);
      break;
    case STATE_CAPTURE_SLICE:
      err = embr_app_handle_state_capture_slice(&runtime);
      break;
    case STATE_WINDOW_CHECK:
      err = embr_app_handle_state_window_check(&runtime);
      break;
    case STATE_RECOVER:
      err = embr_app_handle_state_recover(&runtime);
      break;
    default:
      __ASSERT(false, "Invalid app state");
      return -EINVAL;
    }
    if (err) {
      return err;
    }
  }
}
