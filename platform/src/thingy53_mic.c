#include "thingy53_mic.h"
#include "thingy53_mic_impl.h"

int thingy53_mic_init(void) {
  int err = thingy53_mic_init_impl();
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_mic_start(void) {
  int err = thingy53_mic_start_impl();
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_mic_stop(void) {
  int err = thingy53_mic_stop_impl();
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_mic_deinit(void) {
  int err = thingy53_mic_deinit_impl();
  if (err) {
    return err;
  }

  return 0;
}

int thingy53_mic_reset(void) {
  int err = thingy53_mic_reset_impl();
  if (err) {
    return err;
  }

  return 0;
}
