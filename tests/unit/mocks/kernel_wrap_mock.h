#ifndef KERNEL_WRAP_MOCK_H_
#define KERNEL_WRAP_MOCK_H_

/**
 * @file kernel_wrap_mock.h
 * @brief Mock helpers for kernel wrapper API.
 */

#include <stdint.h>

struct k_sem;
struct k_work;
typedef void (*kernel_work_handler_t)(struct k_work *work);

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_kernel_wrap_sem_take_forever_return_value;
extern int mock_kernel_wrap_sem_take_forever_call_count;
extern int mock_kernel_wrap_sem_take_timeout_return_value;
extern int mock_kernel_wrap_sem_take_timeout_call_count;
extern int32_t mock_kernel_wrap_sem_take_timeout_last_ms;
extern int mock_kernel_wrap_work_init_call_count;
extern int mock_kernel_wrap_work_submit_call_count;
extern int mock_kernel_wrap_work_submit_return_value;
extern int mock_kernel_wrap_work_init_conflict_count;
extern int mock_kernel_wrap_work_init_return_value;
extern kernel_work_handler_t mock_kernel_wrap_work_last_handler;
void kernel_wrap_mock_run_last_work(void);

void kernel_wrap_mock_reset(void);

int kernel_wrap_sem_take_forever(struct k_sem *sem);
int kernel_wrap_sem_take_timeout_ms(struct k_sem *sem, int32_t timeout_ms);
int kernel_wrap_work_init(kernel_work_handler_t handler);
int kernel_wrap_work_submit(void);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_WRAP_MOCK_H_ */
