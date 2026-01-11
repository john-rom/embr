#include "embr_error_id.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(embr_error);

static atomic_t pending_bits;
static atomic_t work_scheduled;

static void embr_error_work_handler(struct k_work *work);
K_WORK_DEFINE(embr_error_work, embr_error_work_handler);

static void embr_error_work_handler(struct k_work *work) {
  ARG_UNUSED(work);

  atomic_clear(&work_scheduled);
  atomic_t bits = atomic_set(&pending_bits, 0);

  for (int i = 0; i < EMBR_ERR_ID_COUNT; i++) {
    if (bits & BIT(i)) {
      switch ((embr_error_id_t)i) {
      case EMBR_ERR_ID_LED_TOGGLE:
        LOG_ERR("LED toggle failed");
        break;
      default:
        LOG_ERR("Unknown error id: %d", i);
        break;
      }
    }
  }

  if (atomic_get(&pending_bits) != 0) {
    if (atomic_cas(&work_scheduled, 0, 1)) {
      k_work_submit(&embr_error_work);
    }
  }
}

void embr_error_report(embr_error_id_t id) {
  __ASSERT((int)id >= 0 && id < EMBR_ERR_ID_COUNT, "invalid error id");
  if ((int)id < 0 || id >= EMBR_ERR_ID_COUNT) {
    return;
  }

  atomic_or(&pending_bits, BIT(id));

  if (atomic_cas(&work_scheduled, 0, 1)) {
    k_work_submit(&embr_error_work);
  }
}
