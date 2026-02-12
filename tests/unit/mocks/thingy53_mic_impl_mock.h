#ifndef THINGY53_MIC_IMPL_MOCK_H_
#define THINGY53_MIC_IMPL_MOCK_H_

/**
 * @file thingy53_mic_impl_mock.h
 * @brief Mock helpers for Thingy53 mic implementation.
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int mock_thingy53_mic_init_impl_call_count;
extern int mock_thingy53_mic_init_impl_return_value;
extern int mock_thingy53_mic_start_impl_call_count;
extern int mock_thingy53_mic_start_impl_return_value;
extern int mock_thingy53_mic_stop_impl_call_count;
extern int mock_thingy53_mic_stop_impl_return_value;
extern int mock_thingy53_mic_reset_impl_call_count;
extern int mock_thingy53_mic_reset_impl_return_value;
extern int mock_thingy53_mic_deinit_impl_call_count;
extern int mock_thingy53_mic_deinit_impl_return_value;
extern int mock_thingy53_mic_read_impl_call_count;
extern int mock_thingy53_mic_read_impl_return_value;

void thingy53_mic_impl_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_IMPL_MOCK_H_ */
