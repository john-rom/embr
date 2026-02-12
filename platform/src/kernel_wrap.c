#include "kernel_wrap.h"

#include <errno.h>
#include <stdbool.h>
#include <zephyr/kernel.h>

int kernel_wrap_sem_take_forever(struct k_sem *sem) {
  return k_sem_take(sem, K_FOREVER);
}

int kernel_wrap_sem_take_timeout_ms(struct k_sem *sem, int32_t timeout_ms) {
  return k_sem_take(sem, K_MSEC(timeout_ms));
}

struct kernel_wrap_work {
  struct k_work work;
};

static struct kernel_wrap_work error_work_item;
static bool error_work_initialized;
static kernel_work_handler_t error_work_handler;

int kernel_wrap_work_init(kernel_work_handler_t handler) {
  if (!handler) {
    return -EINVAL;
  }

  if (error_work_initialized && error_work_handler != handler) {
    return -EALREADY;
  }

  if (!error_work_initialized) {
    error_work_handler = handler;
    k_work_init(&error_work_item.work, handler);
    error_work_initialized = true;
  }

  return 0;
}

int kernel_wrap_work_submit(void) {
  if (!error_work_initialized) {
    return -ENODEV;
  }

  return k_work_submit(&error_work_item.work);
}
