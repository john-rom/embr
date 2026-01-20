#include <errno.h>
#include <zephyr/audio/dmic.h>
#include <zephyr/device.h>

#include "dmic_wrap.h"

#define DMIC_WRAP_RATE_16K 16000UL

static struct pcm_stream_cfg stream_cfg;
static struct dmic_cfg dmic_cfg_internal;

int dmic_wrap_configure(const struct device *dev) {
  stream_cfg.pcm_rate = DMIC_WRAP_RATE_16K;
  dmic_cfg_internal.streams = &stream_cfg;

  return dmic_configure(dev, &dmic_cfg_internal);
}

int dmic_wrap_trigger(const struct device *dev, dmic_trigger_t cmd) {
  switch (cmd) {
  case DMIC_WRAP_TRIGGER_STOP:
    return dmic_trigger(dev, DMIC_TRIGGER_STOP);
  case DMIC_WRAP_TRIGGER_START:
    return dmic_trigger(dev, DMIC_TRIGGER_START);
  case DMIC_WRAP_TRIGGER_PAUSE:
    return dmic_trigger(dev, DMIC_TRIGGER_PAUSE);
  case DMIC_WRAP_TRIGGER_RELEASE:
    return dmic_trigger(dev, DMIC_TRIGGER_RELEASE);
  case DMIC_WRAP_TRIGGER_RESET:
    return dmic_trigger(dev, DMIC_TRIGGER_RESET);
  default:
    return -EINVAL;
  }
}
