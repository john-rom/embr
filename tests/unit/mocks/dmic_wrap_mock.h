#ifndef DMIC_WRAP_MOCK_H_
#define DMIC_WRAP_MOCK_H_

/**
 * @file dmic_wrap_mock.h
 * @brief Mock helpers for DMIC wrapper API.
 */

#include "dmic_wrap.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_dmic_wrap_configure_return_value;
extern int mock_dmic_wrap_trigger_return_value;
extern int mock_dmic_wrap_configure_call_count;
extern int mock_dmic_wrap_trigger_call_count;
extern dmic_trigger_t mock_dmic_wrap_last_trigger_cmd;
extern int mock_dmic_wrap_read_call_count;
extern int mock_dmic_wrap_read_return_value;

void dmic_wrap_mock_reset(void);

int dmic_wrap_configure(const struct device *dev);
int dmic_wrap_trigger(const struct device *dev, dmic_trigger_t cmd);
int dmic_wrap_read(const struct device *dev, uint8_t stream, void **buffer,
                   size_t *size, int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* DMIC_WRAP_MOCK_H_ */
