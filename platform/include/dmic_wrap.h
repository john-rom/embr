#ifndef DMIC_WRAP_H_
#define DMIC_WRAP_H_

/**
 * @file dmic_wrap.h
 * @brief DMIC wrapper API for unit-test compatibility
 *
 * Thin wrappers around Zephyr DMIC functions, used to enable mocking in tests.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct device;
typedef enum {
  DMIC_WRAP_TRIGGER_STOP,
  DMIC_WRAP_TRIGGER_START,
  DMIC_WRAP_TRIGGER_PAUSE,
  DMIC_WRAP_TRIGGER_RELEASE,
  DMIC_WRAP_TRIGGER_RESET
} dmic_trigger_t;

/**
 * @brief Configure a DMIC device with default stream settings.
 *
 * @param dev Zephyr device handle for the DMIC.
 * @return 0 on success, or -errno on failure.
 */
int dmic_wrap_configure(const struct device *dev);

/**
 * @brief Control DMIC streaming state.
 *
 * @param dev Zephyr device handle for the DMIC.
 * @param cmd Trigger command (start/stop/pause/release/reset).
 * @return 0 on success, or -errno on failure.
 */
int dmic_wrap_trigger(const struct device *dev, dmic_trigger_t cmd);

#ifdef __cplusplus
}
#endif

#endif /* DMIC_WRAP_H_ */
