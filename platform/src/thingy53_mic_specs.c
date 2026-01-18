#include <zephyr/device.h>

#include "thingy53_mic_specs.h"

const struct device *thingy53_mic_get_device() {
  const struct device *mic = DEVICE_DT_GET(DT_NODELABEL(vm3011));

  if (!device_is_ready(mic)) {
    return NULL;
  }

  return mic;
}
