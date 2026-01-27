#include "thingy53_mic_impl_mock.h"
#include "thingy53_mic_impl.h"

int init_impl_call_count = 0;
int init_impl_return_value = 0;
int start_impl_call_count = 0;
int start_impl_return_value = 0;
int stop_impl_call_count = 0;
int stop_impl_return_value = 0;
int reset_impl_call_count = 0;
int reset_impl_return_value = 0;
int deinit_impl_call_count = 0;
int deinit_impl_return_value = 0;

void thingy53_mic_impl_mock_reset(void) {
  init_impl_call_count = 0;
  init_impl_return_value = 0;
  start_impl_call_count = 0;
  start_impl_return_value = 0;
  stop_impl_call_count = 0;
  stop_impl_return_value = 0;
  reset_impl_call_count = 0;
  reset_impl_return_value = 0;
  deinit_impl_call_count = 0;
  deinit_impl_return_value = 0;
}

int thingy53_mic_init_impl(void) {
  init_impl_call_count++;
  return init_impl_return_value;
}

int thingy53_mic_start_impl(void) {
  start_impl_call_count++;
  return start_impl_return_value;
}

int thingy53_mic_stop_impl(void) {
  stop_impl_call_count++;
  return stop_impl_return_value;
}

int thingy53_mic_reset_impl(void) {
  reset_impl_call_count++;
  return reset_impl_return_value;
}

int thingy53_mic_deinit_impl(void) {
  deinit_impl_call_count++;
  return deinit_impl_return_value;
}
