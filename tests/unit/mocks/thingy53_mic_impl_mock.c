#include "thingy53_mic_impl_mock.h"
#include "thingy53_mic_impl.h"

int init_impl_call_count = 0;
int init_impl_return_value = 0;
int start_impl_call_count = 0;
int start_impl_return_value = 0;

void thingy53_mic_impl_mock_reset(void) {
  init_impl_call_count = 0;
  init_impl_return_value = 0;
  start_impl_call_count = 0;
  start_impl_return_value = 0;
}

int thingy53_mic_init_impl(void) {
  init_impl_call_count++;
  return init_impl_return_value;
}

int thingy53_mic_start_impl(void) {
  start_impl_call_count++;
  return start_impl_return_value;
}
