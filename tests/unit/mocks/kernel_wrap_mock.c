#include "kernel_wrap_mock.h"

#include <errno.h>
#include <stddef.h>

int mock_kernel_wrap_sem_take_forever_return_value = 0;
int mock_kernel_wrap_sem_take_forever_call_count = 0;
int mock_kernel_wrap_sem_take_timeout_return_value = 0;
int mock_kernel_wrap_sem_take_timeout_call_count = 0;
int32_t mock_kernel_wrap_sem_take_timeout_last_ms = 0;
int mock_kernel_wrap_work_init_call_count = 0;
int mock_kernel_wrap_work_submit_call_count = 0;
int mock_kernel_wrap_work_submit_return_value = 0;
int mock_kernel_wrap_work_init_conflict_count = 0;
int mock_kernel_wrap_work_init_return_value = 0;
kernel_work_handler_t mock_kernel_wrap_work_last_handler = NULL;

void kernel_wrap_mock_reset(void) {
  mock_kernel_wrap_sem_take_forever_return_value = 0;
  mock_kernel_wrap_sem_take_forever_call_count = 0;
  mock_kernel_wrap_sem_take_timeout_return_value = 0;
  mock_kernel_wrap_sem_take_timeout_call_count = 0;
  mock_kernel_wrap_sem_take_timeout_last_ms = 0;
  mock_kernel_wrap_work_init_call_count = 0;
  mock_kernel_wrap_work_submit_call_count = 0;
  mock_kernel_wrap_work_submit_return_value = 0;
  mock_kernel_wrap_work_init_conflict_count = 0;
  mock_kernel_wrap_work_init_return_value = 0;
  mock_kernel_wrap_work_last_handler = NULL;
}

int kernel_wrap_sem_take_forever(struct k_sem *sem) {
  (void)sem;
  mock_kernel_wrap_sem_take_forever_call_count++;
  return mock_kernel_wrap_sem_take_forever_return_value;
}

int kernel_wrap_sem_take_timeout_ms(struct k_sem *sem, int32_t timeout_ms) {
  (void)sem;
  mock_kernel_wrap_sem_take_timeout_call_count++;
  mock_kernel_wrap_sem_take_timeout_last_ms = timeout_ms;
  return mock_kernel_wrap_sem_take_timeout_return_value;
}

int kernel_wrap_work_init(kernel_work_handler_t handler) {
  mock_kernel_wrap_work_init_call_count++;

  if (mock_kernel_wrap_work_init_return_value) {
    return mock_kernel_wrap_work_init_return_value;
  }

  if (!handler) {
    return -EINVAL;
  }
  if (mock_kernel_wrap_work_last_handler &&
      mock_kernel_wrap_work_last_handler != handler) {
    mock_kernel_wrap_work_init_conflict_count++;
    return -EALREADY;
  }
  mock_kernel_wrap_work_last_handler = handler;
  return 0;
}

int kernel_wrap_work_submit(void) {
  mock_kernel_wrap_work_submit_call_count++;
  return mock_kernel_wrap_work_submit_return_value;
}

void kernel_wrap_mock_run_last_work(void) {
  if (mock_kernel_wrap_work_last_handler) {
    mock_kernel_wrap_work_last_handler(NULL);
  }
}
