#include <errno.h>
#include <stdint.h>
#include <zephyr/audio/dmic.h>
#include <zephyr/device.h>

#include "dmic_wrap.h"

static struct pcm_stream_cfg stream_cfg;
static struct dmic_cfg dmic_cfg_internal;

int dmic_wrap_configure(const struct device *dev) {
  stream_cfg.pcm_rate = DMIC_WRAP_SAMPLE_RATE_HZ;
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

int dmic_wrap_read(const struct device *dev, uint8_t stream, void **buffer,
                   size_t *size, int32_t timeout) {
  return dmic_read(dev, stream, buffer, size, timeout);
}
