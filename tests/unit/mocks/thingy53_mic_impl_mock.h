#ifndef THINGY53_MIC_IMPL_MOCK_H_
#define THINGY53_MIC_IMPL_MOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int init_impl_call_count;
extern int init_impl_return_value;
extern int start_impl_call_count;
extern int start_impl_return_value;
extern int stop_impl_call_count;
extern int stop_impl_return_value;
extern int reset_impl_call_count;
extern int reset_impl_return_value;
extern int deinit_impl_call_count;
extern int deinit_impl_return_value;

void thingy53_mic_impl_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* THINGY53_MIC_IMPL_MOCK_H_ */
