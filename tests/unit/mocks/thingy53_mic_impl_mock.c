#include "thingy53_mic_impl_mock.h"
#include "thingy53_mic_impl.h"

int init_call_count = 0;
int init_return_value = 0;

void thingy53_mic_impl_mock_reset(void) {
  init_call_count = 0;
  init_return_value = 0;
}

int thingy53_mic_init_impl(void) {
  init_call_count++;
  return init_return_value;
}
