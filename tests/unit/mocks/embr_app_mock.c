#include "embr_app_mock.h"

int mock_embr_app_led_toggle_on_capture_start_return_value = 0;
int mock_embr_app_led_toggle_on_capture_end_async_return_value = 0;
int mock_embr_app_led_toggle_on_capture_start_call_count = 0;
int mock_embr_app_led_toggle_on_capture_end_async_call_count = 0;
void *mock_embr_app_wos_sem = (void *)0x1;
void *mock_embr_app_pdm_sem = (void *)0x2;

void embr_app_mock_reset(void) {
  mock_embr_app_led_toggle_on_capture_start_return_value = 0;
  mock_embr_app_led_toggle_on_capture_end_async_return_value = 0;
  mock_embr_app_led_toggle_on_capture_start_call_count = 0;
  mock_embr_app_led_toggle_on_capture_end_async_call_count = 0;
  mock_embr_app_wos_sem = (void *)0x1;
  mock_embr_app_pdm_sem = (void *)0x2;
}

int embr_app_led_toggle_on_capture_start(void) {
  mock_embr_app_led_toggle_on_capture_start_call_count++;
  return mock_embr_app_led_toggle_on_capture_start_return_value;
}

int embr_app_led_toggle_on_capture_end_async(void) {
  mock_embr_app_led_toggle_on_capture_end_async_call_count++;
  return mock_embr_app_led_toggle_on_capture_end_async_return_value;
}

struct k_sem *embr_app_wos_sem(void) { return mock_embr_app_wos_sem; }

struct k_sem *embr_app_pdm_sem(void) { return mock_embr_app_pdm_sem; }
