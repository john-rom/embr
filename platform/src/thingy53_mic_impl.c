#include <errno.h>
#include <stddef.h>
#include <zephyr/sys/__assert.h>

#include "dmic_wrap.h"
#include "thingy53_mic_impl.h"
#include "thingy53_mic_specs.h"

static const struct device *mic = NULL;
static bool mic_initialized = false;
static bool mic_streaming = false;

int thingy53_mic_init_impl(void) {
  if (mic_streaming) {
    return -EALREADY;
  }
  if (mic_initialized) {
    return 0;
  }

  mic = thingy53_mic_get_device();
  if (!mic) {
    return -ENODEV;
  }

  int err = dmic_wrap_configure(mic);
  if (err) {
    return err;
  }

  mic_initialized = true;
  return 0;
}

int thingy53_mic_start_impl(void) {
  __ASSERT(mic_initialized, "DMIC not initialized");
  if (!mic_initialized) {
    return -ENODEV;
  }
  int err = dmic_wrap_trigger(mic, DMIC_WRAP_TRIGGER_START);
  if (err) {
    return err;
  }

  mic_streaming = true;

  return 0;
}

int thingy53_mic_stop_impl(void) {
  __ASSERT(mic_initialized, "DMIC not initialized");
  if (!mic_initialized) {
    return -ENODEV;
  }
  int err = dmic_wrap_trigger(mic, DMIC_WRAP_TRIGGER_STOP);
  if (err) {
    return err;
  }

  mic_streaming = false;

  return 0;
}

int thingy53_mic_deinit_impl(void) {
  if (mic_initialized && mic_streaming) {
    int err = dmic_wrap_trigger(mic, DMIC_WRAP_TRIGGER_STOP);
    if (err) {
      return err;
    }
  }
  mic = NULL;
  mic_initialized = false;
  mic_streaming = false;
  return 0;
}

int thingy53_mic_reset_impl(void) {
  __ASSERT(mic_initialized, "DMIC not initialized");
  if (!mic_initialized) {
    return -ENODEV;
  }
  int err = dmic_wrap_trigger(mic, DMIC_WRAP_TRIGGER_RESET);
  if (err) {
    return err;
  }

  mic_streaming = false;

  return 0;
}
