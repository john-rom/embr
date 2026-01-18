#ifndef THINGY53_MIC_SPECS_H_
#define THINGY53_MIC_SPECS_H_

/**
 * @file thingy53_mic_specs.h
 * @brief Accessor for Thingy53 DMIC device.
 *
 * Provides a devicetree-backed DMIC device.
 */

#ifdef __cplusplus
extern "C" {
#endif

struct device;

/**
 * @brief Get the Thingy53 DMIC device.
 *
 * @return Pointer to the Thingy53 DMIC device.
 */
const struct device *thingy53_mic_get_device(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_SPECS_H_ */
