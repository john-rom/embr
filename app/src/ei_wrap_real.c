#include <ei_wrapper.h>
#include <stddef.h>

#include "ei_wrap.h"

int ei_wrap_init(ei_wrapper_result_ready_cb cb) { return ei_wrapper_init(cb); }

int ei_wrap_start_prediction(size_t window_shift, size_t frame_shift) {
  return ei_wrapper_start_prediction(window_shift, frame_shift);
}

int ei_wrap_add_data(const float *data, size_t data_size) {
  return ei_wrapper_add_data(data, data_size);
}

int ei_wrap_get_next_classification_result(const char **label, float *value,
                                           size_t *idx) {
  return ei_wrapper_get_next_classification_result(label, value, idx);
}

int ei_wrap_classifier_has_anomaly(void) {
  return ei_wrapper_classifier_has_anomaly();
}

int ei_wrap_get_anomaly(float *anomaly) {
  return ei_wrapper_get_anomaly(anomaly);
}
