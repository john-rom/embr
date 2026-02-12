#include <errno.h>
#include <stdint.h>
#include <zephyr/ztest.h>

#include "embr_error_id.h"
#include "kernel_wrap_mock.h"

static volatile int handler_call_count;
static volatile embr_error_id_t last_id;

static void test_error_handler(embr_error_id_t id) {
  handler_call_count++;
  last_id = id;
}

static void embr_error_id_before(void *fixture) {
  (void)fixture;
  handler_call_count = 0;
  last_id = EMBR_ERR_ID_COUNT;
  kernel_wrap_mock_reset();
  embr_error_register_handler(test_error_handler);
  embr_error_reset_stats();
}

ZTEST_SUITE(embr_error_id, NULL, NULL, embr_error_id_before, NULL, NULL);

ZTEST(embr_error_id, test_error_id_to_str_returns_message_success) {
  zassert_not_null(embr_error_id_to_str(EMBR_ERR_ID_LED_TOGGLE),
                   "LED_TOGGLE string should not be NULL");
  zassert_not_null(embr_error_id_to_str(EMBR_ERR_ID_PDM_OVERFLOW),
                   "PDM_OVERFLOW string should not be NULL");
  zassert_not_null(embr_error_id_to_str(EMBR_ERR_ID_PDM_SET_BUFFER),
                   "PDM_SET_BUFFER string should not be NULL");
}

ZTEST(embr_error_id, test_error_report_invokes_handler_success) {
  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  kernel_wrap_mock_run_last_work();

  zassert_equal(handler_call_count, 1,
                "embr_error_report should invoke handler once");
  zassert_equal(last_id, EMBR_ERR_ID_PDM_OVERFLOW,
                "handler should receive PDM_OVERFLOW");
  zassert_equal(mock_kernel_wrap_work_submit_call_count, 1,
                "kernel_wrap_work_submit should be called once");
}

ZTEST(embr_error_id, test_error_report_coalesces_ids_success) {
  handler_call_count = 0;
  last_id = EMBR_ERR_ID_COUNT;

  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  kernel_wrap_mock_run_last_work();

  zassert_equal(handler_call_count, 1,
                "duplicate error IDs should be coalesced");

  handler_call_count = 0;
  last_id = EMBR_ERR_ID_COUNT;

  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  embr_error_report(EMBR_ERR_ID_PDM_SET_BUFFER);
  kernel_wrap_mock_run_last_work();

  zassert_equal(handler_call_count, 2,
                "distinct error IDs should be reported separately");
}

ZTEST(embr_error_id,
      test_error_report_submit_fail_then_retry_resubmits_and_invokes_handler_success) {
  mock_kernel_wrap_work_submit_return_value = -EIO;

  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  zassert_equal(mock_kernel_wrap_work_submit_call_count, 3,
                "first submit should be attempted up to retry limit");
  zassert_equal(handler_call_count, 0,
                "handler should not run when submit fails");

  mock_kernel_wrap_work_submit_return_value = 0;

  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  zassert_equal(mock_kernel_wrap_work_submit_call_count, 4,
                "second submit should be attempted after previous submit failure");

  kernel_wrap_mock_run_last_work();

  zassert_equal(handler_call_count, 1,
                "handler should run once after successful resubmit");
  zassert_equal(last_id, EMBR_ERR_ID_PDM_OVERFLOW,
                "handler should receive PDM_OVERFLOW");
}

ZTEST(embr_error_id, test_error_stats_counts_init_fail_and_submit_fail_success) {
  struct embr_error_stats stats = {0};

  mock_kernel_wrap_work_init_return_value = -EIO;
  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);

  mock_kernel_wrap_work_init_return_value = 0;
  mock_kernel_wrap_work_submit_return_value = -EIO;
  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);

  embr_error_get_stats(&stats);

  zassert_equal(stats.work_init_fail_count, 1,
                "work_init_fail_count should increment on init failure");
  zassert_equal(stats.work_submit_fail_count, 3,
                "work_submit_fail_count should increment per retry attempt");
}

ZTEST(embr_error_id, test_error_stats_reset_clears_counts_success) {
  struct embr_error_stats stats = {0};

  mock_kernel_wrap_work_submit_return_value = -EIO;
  embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);

  embr_error_reset_stats();
  embr_error_get_stats(&stats);

  zassert_equal(stats.work_init_fail_count, 0,
                "work_init_fail_count should be 0 after reset");
  zassert_equal(stats.work_submit_fail_count, 0,
                "work_submit_fail_count should be 0 after reset");
}

ZTEST(embr_error_id, test_error_stats_submit_fail_count_saturates_at_u8_max_success) {
  struct embr_error_stats stats = {0};

  mock_kernel_wrap_work_submit_return_value = -EIO;

  for (int i = 0; i < 100; i++) {
    embr_error_report(EMBR_ERR_ID_PDM_OVERFLOW);
  }

  embr_error_get_stats(&stats);
  zassert_equal(stats.work_submit_fail_count, UINT8_MAX,
                "work_submit_fail_count should saturate at UINT8_MAX");
}
