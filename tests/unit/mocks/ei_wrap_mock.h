#ifndef EI_WRAP_MOCK_H
#define EI_WRAP_MOCK_H

/**
 * @file ei_wrap_mock.h
 * @brief Mock helpers for Edge Impulse wrapper API.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_ei_wrap_start_prediction_return_value;
extern int mock_ei_wrap_start_prediction_call_count;
extern int mock_ei_wrap_add_data_return_value;
extern int mock_ei_wrap_add_data_call_count;
extern int mock_ei_wrap_get_next_result_return_value;
extern int mock_ei_wrap_get_next_result_call_count;
extern int mock_ei_wrap_classifier_has_anomaly_return_value;
extern int mock_ei_wrap_classifier_has_anomaly_call_count;
extern int mock_ei_wrap_get_anomaly_return_value;
extern int mock_ei_wrap_get_anomaly_call_count;
extern float mock_ei_wrap_get_anomaly_value;

void ei_wrap_mock_reset(void);

int ei_wrap_start_prediction(size_t window_shift, size_t frame_shift);
int ei_wrap_add_data(const float *data, size_t data_size);
int ei_wrap_get_next_classification_result(const char **label, float *value,
                                           size_t *idx);
int ei_wrap_classifier_has_anomaly(void);
int ei_wrap_get_anomaly(float *anomaly);

#ifdef __cplusplus
}
#endif

#endif /* EI_WRAP_MOCK_H */
