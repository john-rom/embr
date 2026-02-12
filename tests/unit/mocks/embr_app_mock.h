#ifndef EMBR_APP_MOCK_H_
#define EMBR_APP_MOCK_H_

/**
 * @file embr_app_mock.h
 * @brief Mock helpers for embr app layer.
 */

struct k_sem;

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_embr_app_led_toggle_on_capture_start_return_value;
extern int mock_embr_app_led_toggle_on_capture_end_async_return_value;
extern int mock_embr_app_led_toggle_on_capture_start_call_count;
extern int mock_embr_app_led_toggle_on_capture_end_async_call_count;
extern void *mock_embr_app_wos_sem;
extern void *mock_embr_app_pdm_sem;

void embr_app_mock_reset(void);

int embr_app_led_toggle_on_capture_start(void);
int embr_app_led_toggle_on_capture_end_async(void);
struct k_sem *embr_app_wos_sem(void);
struct k_sem *embr_app_pdm_sem(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_APP_MOCK_H_ */
