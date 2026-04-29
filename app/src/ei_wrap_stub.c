#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

#include "dmic_wrap.h"
#include "ei_wrap.h"

#define EI_WRAP_STUB_LABEL "unknown"
#define EI_WRAP_STUB_SCORE 1.0f

#define EI_WRAP_STUB_SAMPLES_PER_SLICE CONFIG_VM3011_PDM_BUFFER_SIZE
#define EI_WRAP_STUB_SAMPLES_PER_WINDOW DMIC_WRAP_SAMPLE_RATE_HZ
#define EI_WRAP_STUB_SLICES_PER_WINDOW                                         \
  (EI_WRAP_STUB_SAMPLES_PER_WINDOW / EI_WRAP_STUB_SAMPLES_PER_SLICE)

static ei_wrapper_result_ready_cb result_ready_cb;
static size_t slices_added;
static bool prediction_active;
static bool result_ready;
static bool result_consumed;

int ei_wrap_init(ei_wrapper_result_ready_cb cb) {
  if (cb == NULL) {
    return -EINVAL;
  }

  result_ready_cb = cb;
  slices_added = 0;
  prediction_active = false;
  result_ready = false;
  result_consumed = false;
  return 0;
}

int ei_wrap_start_prediction(size_t window_shift, size_t frame_shift) {
  (void)window_shift;
  (void)frame_shift;

  if (result_ready_cb == NULL) {
    return -EINVAL;
  }

  slices_added = 0;
  prediction_active = true;
  result_ready = false;
  result_consumed = false;
  return 0;
}

int ei_wrap_add_data(const float *data, size_t data_size) {
  (void)data;
  (void)data_size;

  if (!prediction_active) {
    return 0;
  }

  slices_added++;
  if (slices_added < EI_WRAP_STUB_SLICES_PER_WINDOW) {
    return 0;
  }

  prediction_active = false;
  result_ready = true;
  result_consumed = false;
  result_ready_cb(0);
  return 0;
}

int ei_wrap_get_next_classification_result(const char **label, float *value,
                                           size_t *idx) {
  if (label == NULL || value == NULL) {
    return -EINVAL;
  }

  if (!result_ready || result_consumed) {
    return -ENOENT;
  }

  *label = EI_WRAP_STUB_LABEL;
  *value = EI_WRAP_STUB_SCORE;
  if (idx != NULL) {
    *idx = 0;
  }

  result_consumed = true;
  return 0;
}

int ei_wrap_classifier_has_anomaly(void) { return 0; }

int ei_wrap_get_anomaly(float *anomaly) {
  if (anomaly == NULL) {
    return -EINVAL;
  }

  *anomaly = 0.0f;
  return 0;
}
