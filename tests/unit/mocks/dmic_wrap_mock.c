#include "dmic_wrap_mock.h"

int mock_dmic_wrap_configure_return_value = 0;
int mock_dmic_wrap_trigger_return_value = 0;
int mock_dmic_wrap_configure_call_count = 0;
int mock_dmic_wrap_trigger_call_count = 0;
dmic_trigger_t mock_dmic_wrap_last_trigger_cmd = (dmic_trigger_t)-1;
int mock_dmic_wrap_read_call_count = 0;
int mock_dmic_wrap_read_return_value = 0;

void dmic_wrap_mock_reset(void) {
  mock_dmic_wrap_configure_return_value = 0;
  mock_dmic_wrap_trigger_return_value = 0;
  mock_dmic_wrap_configure_call_count = 0;
  mock_dmic_wrap_trigger_call_count = 0;
  mock_dmic_wrap_last_trigger_cmd = (dmic_trigger_t)-1;
  mock_dmic_wrap_read_call_count = 0;
  mock_dmic_wrap_read_return_value = 0;
}

int dmic_wrap_configure(const struct device *dev) {
  mock_dmic_wrap_configure_call_count++;
  return mock_dmic_wrap_configure_return_value;
}

int dmic_wrap_trigger(const struct device *dev, dmic_trigger_t cmd) {
  mock_dmic_wrap_trigger_call_count++;
  mock_dmic_wrap_last_trigger_cmd = cmd;
  return mock_dmic_wrap_trigger_return_value;
}

int dmic_wrap_read(const struct device *dev, uint8_t stream, void **buffer,
                   size_t *size, int32_t timeout) {
  mock_dmic_wrap_read_call_count++;
  return mock_dmic_wrap_read_return_value;
}
