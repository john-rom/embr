#include "ei_wrap_mock.h"

int mock_ei_wrap_start_prediction_return_value = 0;
int mock_ei_wrap_start_prediction_call_count = 0;
int mock_ei_wrap_add_data_return_value = 0;
int mock_ei_wrap_add_data_call_count = 0;
int mock_ei_wrap_get_next_result_return_value = 0;
int mock_ei_wrap_get_next_result_call_count = 0;
int mock_ei_wrap_classifier_has_anomaly_return_value = 0;
int mock_ei_wrap_classifier_has_anomaly_call_count = 0;
int mock_ei_wrap_get_anomaly_return_value = 0;
int mock_ei_wrap_get_anomaly_call_count = 0;
float mock_ei_wrap_get_anomaly_value = 0.0f;

void ei_wrap_mock_reset(void) {
  mock_ei_wrap_start_prediction_return_value = 0;
  mock_ei_wrap_start_prediction_call_count = 0;
  mock_ei_wrap_add_data_return_value = 0;
  mock_ei_wrap_add_data_call_count = 0;
  mock_ei_wrap_get_next_result_return_value = 0;
  mock_ei_wrap_get_next_result_call_count = 0;
  mock_ei_wrap_classifier_has_anomaly_return_value = 0;
  mock_ei_wrap_classifier_has_anomaly_call_count = 0;
  mock_ei_wrap_get_anomaly_return_value = 0;
  mock_ei_wrap_get_anomaly_call_count = 0;
  mock_ei_wrap_get_anomaly_value = 0.0f;
}

int ei_wrap_start_prediction(size_t window_shift, size_t frame_shift) {
  mock_ei_wrap_start_prediction_call_count++;
  return mock_ei_wrap_start_prediction_return_value;
}

int ei_wrap_add_data(const float *data, size_t data_size) {
  (void)data;
  (void)data_size;
  mock_ei_wrap_add_data_call_count++;
  return mock_ei_wrap_add_data_return_value;
}

int ei_wrap_get_next_classification_result(const char **label, float *value,
                                           size_t *idx) {
  (void)label;
  (void)value;
  (void)idx;
  mock_ei_wrap_get_next_result_call_count++;
  return mock_ei_wrap_get_next_result_return_value;
}

int ei_wrap_classifier_has_anomaly(void) {
  mock_ei_wrap_classifier_has_anomaly_call_count++;
  return mock_ei_wrap_classifier_has_anomaly_return_value;
}

int ei_wrap_get_anomaly(float *anomaly) {
  mock_ei_wrap_get_anomaly_call_count++;
  if (anomaly) {
    *anomaly = mock_ei_wrap_get_anomaly_value;
  }
  return mock_ei_wrap_get_anomaly_return_value;
}
