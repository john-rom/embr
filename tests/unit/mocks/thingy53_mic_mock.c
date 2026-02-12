#include "thingy53_mic_mock.h"

int mock_thingy53_mic_start_return_value = 0;
int mock_thingy53_mic_stop_return_value = 0;
int mock_thingy53_mic_reset_return_value = 0;
int mock_thingy53_mic_read_return_value = 0;
int mock_thingy53_mic_start_call_count = 0;
int mock_thingy53_mic_stop_call_count = 0;
int mock_thingy53_mic_reset_call_count = 0;
int mock_thingy53_mic_read_call_count = 0;
void *mock_thingy53_mic_read_buffer = NULL;
size_t mock_thingy53_mic_read_size = 0;

void thingy53_mic_mock_reset(void) {
  mock_thingy53_mic_start_return_value = 0;
  mock_thingy53_mic_stop_return_value = 0;
  mock_thingy53_mic_reset_return_value = 0;
  mock_thingy53_mic_read_return_value = 0;
  mock_thingy53_mic_start_call_count = 0;
  mock_thingy53_mic_stop_call_count = 0;
  mock_thingy53_mic_reset_call_count = 0;
  mock_thingy53_mic_read_call_count = 0;
  mock_thingy53_mic_read_buffer = NULL;
  mock_thingy53_mic_read_size = 0;
}

int thingy53_mic_start(void) {
  mock_thingy53_mic_start_call_count++;
  return mock_thingy53_mic_start_return_value;
}

int thingy53_mic_stop(void) {
  mock_thingy53_mic_stop_call_count++;
  return mock_thingy53_mic_stop_return_value;
}

int thingy53_mic_reset(void) {
  mock_thingy53_mic_reset_call_count++;
  return mock_thingy53_mic_reset_return_value;
}

int thingy53_mic_read(void **buffer, size_t *size, int32_t timeout) {
  (void)timeout;
  mock_thingy53_mic_read_call_count++;
  if (buffer) {
    *buffer = mock_thingy53_mic_read_buffer;
  }
  if (size) {
    *size = mock_thingy53_mic_read_size;
  }
  return mock_thingy53_mic_read_return_value;
}
