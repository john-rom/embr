#include "embr_error_id.h"

#include <zephyr/sys/__assert.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/util.h>

#include "kernel_wrap.h"

#define EMBR_ERROR_WORK_SUBMIT_RETRY_LIMIT 3

static atomic_t pending_bits;
static atomic_t work_scheduled;
static atomic_t work_init_fail_count;
static atomic_t work_submit_fail_count;
static embr_error_handler_t err_handler;

static void embr_error_work_handler(struct k_work *work);
static void increment_work_fail_counter(atomic_t *counter);
static void submit_work_with_retry(void);

static void increment_work_fail_counter(atomic_t *counter) {
  while (1) {
    atomic_t current_count = atomic_get(counter);
    if (current_count >= UINT8_MAX) {
      return;
    }
    if (atomic_cas(counter, current_count, current_count + 1)) {
      return;
    }
  }
}

static void submit_work_with_retry(void) {
  for (int attempt = 0; attempt < EMBR_ERROR_WORK_SUBMIT_RETRY_LIMIT;
       attempt++) {
    int err = kernel_wrap_work_submit();
    if (!err) {
      return;
    }

    increment_work_fail_counter(&work_submit_fail_count);
    atomic_clear(&work_scheduled);

    if ((attempt + 1) >= EMBR_ERROR_WORK_SUBMIT_RETRY_LIMIT) {
      break;
    }
    if (atomic_get(&pending_bits) == 0) {
      break;
    }
    if (!atomic_cas(&work_scheduled, 0, 1)) {
      return;
    }
  }

  if (atomic_get(&pending_bits) != 0) {
    // Drop stale pending IDs when transport submission repeatedly fails.
    atomic_set(&pending_bits, 0);
  }
}

static void embr_error_work_handler(struct k_work *work) {
  ARG_UNUSED(work);

  atomic_clear(&work_scheduled);
  atomic_t bits = atomic_set(&pending_bits, 0);

  for (int i = 0; i < EMBR_ERR_ID_COUNT; i++) {
    if (bits & BIT(i)) {
      if (err_handler) {
        err_handler((embr_error_id_t)i);
      }
    }
  }

  if (atomic_get(&pending_bits) != 0) {
    if (atomic_cas(&work_scheduled, 0, 1)) {
      submit_work_with_retry();
    }
  }
}

void embr_error_register_handler(embr_error_handler_t handler) {
  err_handler = handler;
}

const char *embr_error_id_to_str(embr_error_id_t id) {
  switch (id) {
  case EMBR_ERR_ID_LED_TOGGLE:
    return "LED toggle failed";
  case EMBR_ERR_ID_PDM_OVERFLOW:
    return "PDM overflow";
  case EMBR_ERR_ID_PDM_SET_BUFFER:
    return "PDM set buffer failed";
  default:
    return "Unknown error id";
  }
}

void embr_error_report(embr_error_id_t id) {
  __ASSERT((int)id >= 0 && id < EMBR_ERR_ID_COUNT, "Invalid error id");
  if ((int)id < 0 || id >= EMBR_ERR_ID_COUNT) {
    return;
  }

  int err = kernel_wrap_work_init(embr_error_work_handler);
  if (err) {
    increment_work_fail_counter(&work_init_fail_count);
    return;
  }

  atomic_or(&pending_bits, BIT(id));

  if (atomic_cas(&work_scheduled, 0, 1)) {
    submit_work_with_retry();
  }
}

void embr_error_get_stats(struct embr_error_stats *out_stats) {
  __ASSERT(out_stats != NULL, "out_stats is null");
  if (out_stats == NULL) {
    return;
  }

  out_stats->work_init_fail_count = (uint8_t)atomic_get(&work_init_fail_count);
  out_stats->work_submit_fail_count =
      (uint8_t)atomic_get(&work_submit_fail_count);
}

void embr_error_reset_stats(void) {
  atomic_clear(&work_init_fail_count);
  atomic_clear(&work_submit_fail_count);
}
