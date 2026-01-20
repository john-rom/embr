#ifndef DMIC_WRAP_MOCK_H_
#define DMIC_WRAP_MOCK_H_

#include "dmic_wrap.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_dmic_wrap_configure_ret;
extern int mock_dmic_wrap_trigger_ret;
extern int mock_dmic_configure_call_count;
extern int mock_dmic_trigger_call_count;

void dmic_wrap_mock_reset(void);

int dmic_wrap_configure(const struct device *dev);
int dmic_wrap_trigger(const struct device *dev, dmic_trigger_t cmd);

#ifdef __cplusplus
}
#endif

#endif /* DMIC_WRAP_MOCK_H_ */
