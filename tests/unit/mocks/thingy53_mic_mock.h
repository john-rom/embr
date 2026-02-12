#ifndef THINGY53_MIC_MOCK_H_
#define THINGY53_MIC_MOCK_H_

/**
 * @file thingy53_mic_mock.h
 * @brief Mock helpers for Thingy53 mic API.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_thingy53_mic_start_return_value;
extern int mock_thingy53_mic_stop_return_value;
extern int mock_thingy53_mic_reset_return_value;
extern int mock_thingy53_mic_read_return_value;
extern int mock_thingy53_mic_start_call_count;
extern int mock_thingy53_mic_stop_call_count;
extern int mock_thingy53_mic_reset_call_count;
extern int mock_thingy53_mic_read_call_count;
extern void *mock_thingy53_mic_read_buffer;
extern size_t mock_thingy53_mic_read_size;

void thingy53_mic_mock_reset(void);

int thingy53_mic_start(void);
int thingy53_mic_stop(void);
int thingy53_mic_reset(void);
int thingy53_mic_read(void **buffer, size_t *size, int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_MOCK_H_ */
