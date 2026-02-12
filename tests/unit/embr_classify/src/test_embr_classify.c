#include <errno.h>
#include <string.h>
#include <zephyr/ztest.h>

#include "embr_classify.h"
#include "embr_types.h"

struct test_result {
  const char *label;
  float probability;
};

static struct test_result test_results[8];
static size_t test_result_count = 0;
static size_t test_result_idx = 0;

static void test_results_reset(void) {
  test_result_count = 0;
  test_result_idx = 0;
  memset(test_results, 0, sizeof(test_results));
}

int embr_classify_get_next_result(embr_classification_results *results) {
  if (results == NULL) {
    return -EINVAL;
  }
  if (test_result_idx >= test_result_count) {
    return -ENOENT;
  }

  results->label = test_results[test_result_idx].label;
  results->probability = test_results[test_result_idx].probability;
  test_result_idx++;
  return 0;
}

static void embr_classify_before(void *fixture) {
  (void)fixture;
  test_results_reset();
}

ZTEST_SUITE(embr_classify, NULL, NULL, embr_classify_before, NULL, NULL);

ZTEST(embr_classify, test_get_all_results_populates_scores_success) {
  test_results[0] = (struct test_result){ .label = "ember_glow",
                                          .probability = 0.6f };
  test_results[1] = (struct test_result){ .label = "ember_sleep",
                                          .probability = 0.1f };
  test_results[2] = (struct test_result){ .label = "ember_flip",
                                          .probability = 0.2f };
  test_results[3] = (struct test_result){ .label = "unknown",
                                          .probability = 0.05f };
  test_results[4] = (struct test_result){ .label = "noise",
                                          .probability = 0.05f };
  test_result_count = 5;

  embr_classification_results results;
  embr_label_scores scores;

  int ret = embr_classify_get_all_results(&results, &scores);
  zassert_equal(ret, 0, "embr_classify_get_all_results should return 0");
  zassert_true(scores.ember_glow == 0.6f, "ember_glow score should match");
  zassert_true(scores.ember_sleep == 0.1f, "ember_sleep score should match");
  zassert_true(scores.ember_flip == 0.2f, "ember_flip score should match");
  zassert_true(scores.unknown == 0.05f, "unknown score should match");
  zassert_true(scores.noise == 0.05f, "noise score should match");
}

ZTEST(embr_classify, test_set_command_selects_glow_success) {
  embr_label_scores scores = {
      .ember_glow = 0.8f,
      .ember_sleep = 0.1f,
      .ember_flip = 0.05f,
      .unknown = 0.03f,
      .noise = 0.02f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, EMBR_GLOW,
                "embr_classify_set_command should select EMBR_GLOW");
}

ZTEST(embr_classify, test_set_command_null_returns_einval_fail) {
  embr_label_scores scores = { 0 };

  int ret = embr_classify_set_command(scores, NULL);
  zassert_equal(ret, -EINVAL,
                "embr_classify_set_command should return -EINVAL");
}
