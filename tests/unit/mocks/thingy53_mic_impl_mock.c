#include <stddef.h>
#include <stdint.h>

#include "thingy53_mic_impl.h"
#include "thingy53_mic_impl_mock.h"

int mock_thingy53_mic_init_impl_call_count = 0;
int mock_thingy53_mic_init_impl_return_value = 0;
int mock_thingy53_mic_start_impl_call_count = 0;
int mock_thingy53_mic_start_impl_return_value = 0;
int mock_thingy53_mic_stop_impl_call_count = 0;
int mock_thingy53_mic_stop_impl_return_value = 0;
int mock_thingy53_mic_reset_impl_call_count = 0;
int mock_thingy53_mic_reset_impl_return_value = 0;
int mock_thingy53_mic_deinit_impl_call_count = 0;
int mock_thingy53_mic_deinit_impl_return_value = 0;
int mock_thingy53_mic_read_impl_call_count = 0;
int mock_thingy53_mic_read_impl_return_value = 0;

void thingy53_mic_impl_mock_reset(void) {
  mock_thingy53_mic_init_impl_call_count = 0;
  mock_thingy53_mic_init_impl_return_value = 0;
  mock_thingy53_mic_start_impl_call_count = 0;
  mock_thingy53_mic_start_impl_return_value = 0;
  mock_thingy53_mic_stop_impl_call_count = 0;
  mock_thingy53_mic_stop_impl_return_value = 0;
  mock_thingy53_mic_reset_impl_call_count = 0;
  mock_thingy53_mic_reset_impl_return_value = 0;
  mock_thingy53_mic_deinit_impl_call_count = 0;
  mock_thingy53_mic_deinit_impl_return_value = 0;
  mock_thingy53_mic_read_impl_call_count = 0;
  mock_thingy53_mic_read_impl_return_value = 0;
}

int thingy53_mic_init_impl(void) {
  mock_thingy53_mic_init_impl_call_count++;
  return mock_thingy53_mic_init_impl_return_value;
}

int thingy53_mic_start_impl(void) {
  mock_thingy53_mic_start_impl_call_count++;
  return mock_thingy53_mic_start_impl_return_value;
}

int thingy53_mic_stop_impl(void) {
  mock_thingy53_mic_stop_impl_call_count++;
  return mock_thingy53_mic_stop_impl_return_value;
}

int thingy53_mic_reset_impl(void) {
  mock_thingy53_mic_reset_impl_call_count++;
  return mock_thingy53_mic_reset_impl_return_value;
}

int thingy53_mic_deinit_impl(void) {
  mock_thingy53_mic_deinit_impl_call_count++;
  return mock_thingy53_mic_deinit_impl_return_value;
}

int thingy53_mic_read_impl(void **buffer, size_t *size, int32_t timeout) {
  mock_thingy53_mic_read_impl_call_count++;
  return mock_thingy53_mic_read_impl_return_value;
}
