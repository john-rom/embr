#include <errno.h>

#include "thingy53_mic_impl.h"
#include "thingy53_mic_specs.h"

int thingy53_mic_init_impl(void) {
  const struct device *mic = thingy53_mic_get_device();
  if (!mic) {
    return -ENODEV;
  }

  return 0;
}
