#include <errno.h>

#include "dmic_wrap.h"
#include "thingy53_mic_impl.h"
#include "thingy53_mic_specs.h"

static const struct device *mic;

int thingy53_mic_init_impl(void) {
  mic = thingy53_mic_get_device();
  if (!mic) {
    return -ENODEV;
  }

  int err = 0;

  err = dmic_wrap_configure(mic);
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_mic_start_impl(void) {
  int err = dmic_wrap_trigger(mic, DMIC_WRAP_TRIGGER_START);
  if (err) {
    return err;
  }

  return 0;
}
