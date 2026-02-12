#ifndef THINGY53_MIC_SPECS_MOCK_H_
#define THINGY53_MIC_SPECS_MOCK_H_

/**
 * @file thingy53_mic_specs_mock.h
 * @brief Mock helpers for Thingy53 mic specs.
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void thingy53_mic_specs_mock_reset();

extern bool mock_thingy53_mic_device_ready;

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_SPECS_MOCK_H_ */
