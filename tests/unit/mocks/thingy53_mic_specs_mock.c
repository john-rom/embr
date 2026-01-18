#include <stdbool.h>
#include <stddef.h>

#include "thingy53_mic_specs.h"
#include "thingy53_mic_specs_mock.h"

struct device {
  uint8_t dummy;
};

static const struct device mic;

bool mock_thingy53_mic_device_ready = 0;

void thingy53_mic_specs_mock_reset(void) { mock_thingy53_mic_device_ready = 0; }

const struct device *thingy53_mic_get_device(void) {
  if (!mock_thingy53_mic_device_ready) {
    return NULL;
  } else {
    return &mic;
  }
}
