#ifndef KERNEL_WRAP_H_
#define KERNEL_WRAP_H_

/**
 * @file kernel_wrap.h
 * @brief Kernel wrapper API for unit-test compatibility.
 *
 * Work-item note: this wrapper currently exposes one global work item
 * dedicated to deferred error reporting. Only one handler may be initialized.
 */

#include <stdint.h>

struct k_sem;
struct k_work;

/** Work handler signature for the global deferred-error work item. */
typedef void (*kernel_work_handler_t)(struct k_work *work);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Take a semaphore with an infinite timeout.
 *
 * @param sem Semaphore to take
 * @return 0 on success, or -errno on failure
 */
int kernel_wrap_sem_take_forever(struct k_sem *sem);

/**
 * @brief Take a semaphore with a millisecond timeout.
 *
 * @param sem Semaphore to take
 * @param timeout_ms Timeout in milliseconds
 * @return 0 on success, or -errno on failure
 */
int kernel_wrap_sem_take_timeout_ms(struct k_sem *sem, int32_t timeout_ms);

/**
 * @brief Initialize the global deferred-error work item.
 *
 * @param handler Work handler callback
 * @return 0 on success, -EALREADY if a different handler is already set,
 *         or -errno on failure
 */
int kernel_wrap_work_init(kernel_work_handler_t handler);

/**
 * @brief Submit the global deferred-error work item.
 *
 * @return 0 on success, or -errno on failure
 */
int kernel_wrap_work_submit(void);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_WRAP_H_ */
