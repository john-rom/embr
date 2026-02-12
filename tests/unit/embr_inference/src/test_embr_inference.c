#include <errno.h>
#include <stdint.h>
#include <zephyr/ztest.h>

#include "ei_wrap_mock.h"
#include "embr_app_mock.h"
#include "kernel_wrap_mock.h"
#include "thingy53_mic_mock.h"
#include "embr_inference.h"

static void embr_inference_before(void *fixture) {
  (void)fixture;
  ei_wrap_mock_reset();
  embr_app_mock_reset();
  kernel_wrap_mock_reset();
  thingy53_mic_mock_reset();
  embr_inference_reset_state();
}

ZTEST_SUITE(embr_inference, NULL, NULL, embr_inference_before, NULL, NULL);

ZTEST(embr_inference, test_start_returns_zero_on_success) {
  mock_ei_wrap_start_prediction_return_value = 0;

  int ret = embr_inference_start();
  zassert_equal(ret, 0, "embr_inference_start should return 0");
}

ZTEST(embr_inference, test_start_returns_error_on_fail) {
  mock_ei_wrap_start_prediction_return_value = -EBUSY;

  int ret = embr_inference_start();
  zassert_equal(ret, -EBUSY, "embr_inference_start should return -EBUSY");
  zassert_equal(
      mock_ei_wrap_start_prediction_call_count, 1,
      "embr_inference_start should call ei_wrap_start_prediction once");
}

ZTEST(embr_inference,
      test_begin_audio_capture_after_wos_returns_zero_on_success) {
  int ret = embr_inference_begin_audio_capture_after_wos();
  zassert_equal(ret, 0,
                "embr_inference_begin_audio_capture_after_wos should return 0");
  zassert_equal(mock_kernel_wrap_sem_take_call_count, 1,
                "kernel_wrap_sem_take_forever should be called once");
  zassert_equal(mock_thingy53_mic_start_call_count, 1,
                "thingy53_mic_start should be called once");
  zassert_equal(mock_embr_app_led_toggle_on_capture_start_call_count, 1,
                "embr_app_led_toggle_on_capture_start should be called once");
}

ZTEST(embr_inference,
      test_begin_audio_capture_after_wos_wos_sem_fail_returns_error_on_fail) {
  mock_kernel_wrap_sem_take_return_value = -EAGAIN;

  int ret = embr_inference_begin_audio_capture_after_wos();
  zassert_equal(ret, -EAGAIN,
                "embr_inference_begin_audio_capture_after_wos should return -EAGAIN");
  zassert_equal(mock_kernel_wrap_sem_take_call_count, 1,
                "kernel_wrap_sem_take_forever should be called once");
  zassert_equal(mock_embr_app_led_toggle_on_capture_start_call_count, 0,
                "embr_app_led_toggle_on_capture_start should not be called");
  zassert_equal(mock_thingy53_mic_start_call_count, 0,
                "thingy53_mic_start should not be called");
}

ZTEST(embr_inference,
      test_process_wos_event_audio_timeout_returns_etimedout_fail) {
  int ret = embr_inference_begin_audio_capture_after_wos();
  zassert_equal(ret, 0,
                "embr_inference_begin_audio_capture_after_wos should return 0");

  mock_kernel_wrap_sem_take_timeout_return_value = -EAGAIN;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, -ETIMEDOUT,
                "embr_inference_process_wos_event_audio should return -ETIMEDOUT");
  zassert_equal(mock_kernel_wrap_sem_take_timeout_call_count, 1,
                "kernel_wrap_sem_take_timeout_ms should be called once");
  zassert_equal(mock_kernel_wrap_sem_take_timeout_last_ms, 500,
                "kernel_wrap_sem_take_timeout_ms should use 500 ms");
}

ZTEST(embr_inference, test_skip_first_slice_then_add_data_success) {
  int ret = embr_inference_begin_audio_capture_after_wos();
  zassert_equal(ret, 0,
                "embr_inference_begin_audio_capture_after_wos should return 0");

  static int16_t pcm[CONFIG_VM3011_PDM_BUFFER_SIZE];
  mock_thingy53_mic_read_buffer = pcm;
  mock_thingy53_mic_read_size = CONFIG_VM3011_PDM_BUFFER_SIZE * sizeof(int16_t);
  mock_kernel_wrap_sem_take_timeout_return_value = 0;

  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "embr_inference_process_wos_event_audio should return 0");
  zassert_equal(mock_ei_wrap_add_data_call_count, 0,
                "ei_wrap_add_data should not be called on first slice");

  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "embr_inference_process_wos_event_audio should return 0");
  zassert_equal(mock_ei_wrap_add_data_call_count, 1,
                "ei_wrap_add_data should be called once after skip");
}

ZTEST(embr_inference, test_move_slice_oversize_returns_einval_fail) {
  static int16_t pcm[(CONFIG_VM3011_PDM_BUFFER_SIZE + 1)];
  size_t too_big = (CONFIG_VM3011_PDM_BUFFER_SIZE + 1) * sizeof(int16_t);

  int ret = embr_inference_move_slice_to_pipeline(pcm, too_big);
  zassert_equal(ret, -EINVAL,
                "embr_inference_move_slice_to_pipeline should return -EINVAL");
  zassert_equal(mock_ei_wrap_add_data_call_count, 0,
                "ei_wrap_add_data should not be called on oversize slice");
}

ZTEST(embr_inference, test_move_slice_null_buffer_returns_einval_fail) {
  int ret = embr_inference_move_slice_to_pipeline(NULL, sizeof(int16_t));
  zassert_equal(ret, -EINVAL,
                "embr_inference_move_slice_to_pipeline should return -EINVAL");
  zassert_equal(mock_ei_wrap_add_data_call_count, 0,
                "ei_wrap_add_data should not be called on null buffer");
}

ZTEST(embr_inference, test_move_slice_unaligned_size_returns_einval_fail) {
  static int16_t pcm[CONFIG_VM3011_PDM_BUFFER_SIZE];
  size_t unaligned_size =
      (CONFIG_VM3011_PDM_BUFFER_SIZE * sizeof(int16_t)) - 1;

  int ret = embr_inference_move_slice_to_pipeline(pcm, unaligned_size);
  zassert_equal(ret, -EINVAL,
                "embr_inference_move_slice_to_pipeline should return -EINVAL");
  zassert_equal(mock_ei_wrap_add_data_call_count, 0,
                "ei_wrap_add_data should not be called on unaligned size");
}

ZTEST(embr_inference, test_move_slice_zero_size_returns_einval_fail) {
  static int16_t pcm[CONFIG_VM3011_PDM_BUFFER_SIZE];

  int ret = embr_inference_move_slice_to_pipeline(pcm, 0);
  zassert_equal(ret, -EINVAL,
                "embr_inference_move_slice_to_pipeline should return -EINVAL");
  zassert_equal(mock_ei_wrap_add_data_call_count, 0,
                "ei_wrap_add_data should not be called on zero-size slice");
}

ZTEST(embr_inference,
      test_process_wos_event_audio_repeated_invalid_frame_returns_etimedout_fail) {
  int ret = embr_inference_begin_audio_capture_after_wos();
  zassert_equal(ret, 0,
                "embr_inference_begin_audio_capture_after_wos should return 0");

  static int16_t pcm[CONFIG_VM3011_PDM_BUFFER_SIZE];
  const size_t expected_bytes = CONFIG_VM3011_PDM_BUFFER_SIZE * sizeof(int16_t);
  const size_t invalid_bytes = expected_bytes - sizeof(int16_t);

  mock_thingy53_mic_read_buffer = pcm;
  mock_kernel_wrap_sem_take_timeout_return_value = 0;

  mock_thingy53_mic_read_size = expected_bytes;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "first call should only consume the initial skip slice");

  mock_thingy53_mic_read_size = invalid_bytes;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "first invalid frame should be tolerated");

  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "second invalid frame should be tolerated");

  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, -ETIMEDOUT,
                "third consecutive invalid frame should return -ETIMEDOUT");
  zassert_equal(mock_ei_wrap_add_data_call_count, 0,
                "ei_wrap_add_data should not be called for invalid frames");
}

ZTEST(embr_inference,
      test_process_wos_event_audio_invalid_frame_count_resets_after_valid_success) {
  int ret = embr_inference_begin_audio_capture_after_wos();
  zassert_equal(ret, 0,
                "embr_inference_begin_audio_capture_after_wos should return 0");

  static int16_t pcm[CONFIG_VM3011_PDM_BUFFER_SIZE];
  const size_t expected_bytes = CONFIG_VM3011_PDM_BUFFER_SIZE * sizeof(int16_t);
  const size_t invalid_bytes = expected_bytes - sizeof(int16_t);

  mock_thingy53_mic_read_buffer = pcm;
  mock_kernel_wrap_sem_take_timeout_return_value = 0;

  mock_thingy53_mic_read_size = expected_bytes;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "first call should only consume the initial skip slice");

  mock_thingy53_mic_read_size = invalid_bytes;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "first invalid frame should be tolerated");
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "second invalid frame should be tolerated");

  mock_thingy53_mic_read_size = expected_bytes;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "valid frame should be accepted");
  zassert_equal(mock_ei_wrap_add_data_call_count, 1,
                "ei_wrap_add_data should be called once for valid frame");

  mock_thingy53_mic_read_size = invalid_bytes;
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "counter should reset: first invalid after valid is tolerated");
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, 0,
                "counter should reset: second invalid after valid is tolerated");
  ret = embr_inference_process_wos_event_audio();
  zassert_equal(ret, -ETIMEDOUT,
                "counter should reset: third invalid after valid should timeout");
}

ZTEST(embr_inference, test_stop_audio_if_window_full_stops_mic_success) {
  static int16_t pcm[CONFIG_VM3011_PDM_BUFFER_SIZE];
  size_t pcm_size = CONFIG_VM3011_PDM_BUFFER_SIZE * sizeof(int16_t);

  for (int i = 0; i < 4; i++) {
    int ret = embr_inference_move_slice_to_pipeline(pcm, pcm_size);
    zassert_equal(ret, 0,
                  "embr_inference_move_slice_to_pipeline should return 0");
  }

  int ret = embr_inference_stop_audio_if_window_full();
  zassert_equal(ret, 1,
                "embr_inference_stop_audio_if_window_full should return 1");
  zassert_equal(mock_thingy53_mic_stop_call_count, 1,
                "thingy53_mic_stop should be called once");
  zassert_equal(mock_thingy53_mic_reset_call_count, 1,
                "thingy53_mic_reset should be called once");
  zassert_equal(mock_embr_app_led_toggle_on_capture_end_async_call_count, 1,
                "embr_app_led_toggle_on_capture_end_async should be called once");
}
