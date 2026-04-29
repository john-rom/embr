#include <errno.h>
#include <string.h>
#include <zephyr/ztest.h>

#include "embr_classify.h"
#include "embr_types.h"

struct test_result {
  int err;
  const char *label;
  float probability;
};

/*
 * Test seam for embr_classify_get_next_result().
 *
 * The production implementation is weak and reads from the EI wrapper. These
 * tests override that seam so embr_classify_get_all_results() can be exercised
 * with a scripted stream of results and fetch errors at the module boundary.
 */
static struct test_result scripted_results[8];
static size_t scripted_result_count = 0;
static size_t scripted_result_idx = 0;

static void scripted_results_reset(void) {
  scripted_result_count = 0;
  scripted_result_idx = 0;
  memset(scripted_results, 0, sizeof(scripted_results));
}

static void scripted_result_add(const char *label, float probability) {
  zassert_true(scripted_result_count < ARRAY_SIZE(scripted_results),
               "scripted result queue should have capacity");
  scripted_results[scripted_result_count] = (struct test_result){
    .err = 0,
    .label = label,
    .probability = probability,
  };
  scripted_result_count++;
}

static void scripted_fetch_error_add(int err) {
  zassert_true(scripted_result_count < ARRAY_SIZE(scripted_results),
               "scripted result queue should have capacity");
  scripted_results[scripted_result_count] = (struct test_result){
    .err = err,
  };
  scripted_result_count++;
}

int embr_classify_get_next_result(embr_classification_results *results) {
  if (results == NULL) {
    return -EINVAL;
  }

  /* End of scripted stream mirrors the production "no more results" case. */
  if (scripted_result_idx >= scripted_result_count) {
    return -ENOENT;
  }

  if (scripted_results[scripted_result_idx].err) {
    return scripted_results[scripted_result_idx++].err;
  }

  results->label = scripted_results[scripted_result_idx].label;
  results->probability = scripted_results[scripted_result_idx].probability;
  scripted_result_idx++;
  return 0;
}

static void embr_classify_before(void *fixture) {
  (void)fixture;
  scripted_results_reset();
}

ZTEST_SUITE(embr_classify, NULL, NULL, embr_classify_before, NULL, NULL);

ZTEST(embr_classify,
      test_get_all_results_populates_scores_until_end_of_stream_success) {
  scripted_result_add("embr_glow", 0.6f);
  scripted_result_add("embr_sleep", 0.1f);
  scripted_result_add("embr_flip", 0.2f);
  scripted_result_add("unknown", 0.05f);
  scripted_result_add("noise", 0.05f);

  embr_classification_results results;
  embr_label_scores scores;

  int ret = embr_classify_get_all_results(&results, &scores);
  zassert_equal(ret, 0, "embr_classify_get_all_results should return 0");
  zassert_true(scores.embr_glow == 0.6f, "embr_glow score should match");
  zassert_true(scores.embr_sleep == 0.1f, "embr_sleep score should match");
  zassert_true(scores.embr_flip == 0.2f, "embr_flip score should match");
  zassert_true(scores.unknown == 0.05f, "unknown score should match");
  zassert_true(scores.noise == 0.05f, "noise score should match");
}

ZTEST(embr_classify,
      test_get_all_results_fetch_error_propagates_error_fail) {
  embr_classification_results results = { 0 };
  embr_label_scores scores = {
    .embr_glow = 1.0f,
    .embr_sleep = 1.0f,
    .embr_flip = 1.0f,
    .unknown = 1.0f,
    .noise = 1.0f,
  };

  scripted_fetch_error_add(-EIO);

  int ret = embr_classify_get_all_results(&results, &scores);
  zassert_equal(ret, -EIO,
                "embr_classify_get_all_results should return fetch error");
  zassert_true(scores.embr_glow == 0.0f, "scores should be reset on entry");
  zassert_true(scores.embr_sleep == 0.0f, "scores should be reset on entry");
  zassert_true(scores.embr_flip == 0.0f, "scores should be reset on entry");
  zassert_true(scores.unknown == 0.0f, "scores should be reset on entry");
  zassert_true(scores.noise == 0.0f, "scores should be reset on entry");
}

ZTEST(embr_classify,
      test_get_all_results_unknown_label_returns_enoent_fail) {
  embr_classification_results results;
  embr_label_scores scores;

  /*
   * Unlike queue exhaustion, which the module treats as successful
   * completion, this case returns -ENOENT because the fetched label is not
   * recognized.
   */
  scripted_result_add("invalid_label", 0.4f);

  int ret = embr_classify_get_all_results(&results, &scores);
  zassert_equal(ret, -ENOENT,
                "embr_classify_get_all_results should return -ENOENT");
}

ZTEST(embr_classify, test_assign_label_scores_glow_success) {
  embr_classification_results results = {
    .label = "embr_glow",
    .probability = 0.6f,
  };
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(&results, &scores);
  zassert_equal(ret, 0,
                "embr_classify_assign_label_scores should return 0");
  zassert_true(scores.embr_glow == 0.6f, "embr_glow score should match");
}

ZTEST(embr_classify, test_assign_label_scores_sleep_success) {
  embr_classification_results results = {
    .label = "embr_sleep",
    .probability = 0.1f,
  };
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(&results, &scores);
  zassert_equal(ret, 0,
                "embr_classify_assign_label_scores should return 0");
  zassert_true(scores.embr_sleep == 0.1f, "embr_sleep score should match");
}

ZTEST(embr_classify, test_assign_label_scores_flip_success) {
  embr_classification_results results = {
    .label = "embr_flip",
    .probability = 0.2f,
  };
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(&results, &scores);
  zassert_equal(ret, 0,
                "embr_classify_assign_label_scores should return 0");
  zassert_true(scores.embr_flip == 0.2f, "embr_flip score should match");
}

ZTEST(embr_classify, test_assign_label_scores_unknown_success) {
  embr_classification_results results = {
    .label = "unknown",
    .probability = 0.05f,
  };
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(&results, &scores);
  zassert_equal(ret, 0,
                "embr_classify_assign_label_scores should return 0");
  zassert_true(scores.unknown == 0.05f, "unknown score should match");
}

ZTEST(embr_classify, test_assign_label_scores_noise_success) {
  embr_classification_results results = {
    .label = "noise",
    .probability = 0.05f,
  };
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(&results, &scores);
  zassert_equal(ret, 0,
                "embr_classify_assign_label_scores should return 0");
  zassert_true(scores.noise == 0.05f, "noise score should match");
}

ZTEST(embr_classify, test_assign_label_scores_null_results_returns_einval_fail) {
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(NULL, &scores);
  zassert_equal(ret, -EINVAL,
                "embr_classify_assign_label_scores should return -EINVAL");
}

ZTEST(embr_classify, test_assign_label_scores_null_scores_returns_einval_fail) {
  embr_classification_results results = {
    .label = "embr_glow",
    .probability = 0.6f,
  };

  int ret = embr_classify_assign_label_scores(&results, NULL);
  zassert_equal(ret, -EINVAL,
                "embr_classify_assign_label_scores should return -EINVAL");
}

ZTEST(embr_classify, test_assign_label_scores_unknown_label_returns_enoent_fail) {
  embr_classification_results results = {
    .label = "invalid_label",
    .probability = 0.6f,
  };
  embr_label_scores scores = { 0 };

  int ret = embr_classify_assign_label_scores(&results, &scores);
  zassert_equal(ret, -ENOENT,
                "embr_classify_assign_label_scores should return -ENOENT");
}

ZTEST(embr_classify, test_set_command_selects_glow_success) {
  embr_label_scores scores = {
      .embr_glow = 0.8f,
      .embr_sleep = 0.1f,
      .embr_flip = 0.05f,
      .unknown = 0.03f,
      .noise = 0.02f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, EMBR_GLOW,
                "embr_classify_set_command should select EMBR_GLOW");
}

ZTEST(embr_classify, test_set_command_selects_sleep_success) {
  embr_label_scores scores = {
    .embr_glow = 0.1f,
    .embr_sleep = 0.8f,
    .embr_flip = 0.05f,
    .unknown = 0.03f,
    .noise = 0.02f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, EMBR_SLEEP,
                "embr_classify_set_command should select EMBR_SLEEP");
}

ZTEST(embr_classify, test_set_command_selects_flip_success) {
  embr_label_scores scores = {
    .embr_glow = 0.1f,
    .embr_sleep = 0.05f,
    .embr_flip = 0.8f,
    .unknown = 0.03f,
    .noise = 0.02f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, EMBR_FLIP,
                "embr_classify_set_command should select EMBR_FLIP");
}

ZTEST(embr_classify, test_set_command_selects_unknown_success) {
  embr_label_scores scores = {
    .embr_glow = 0.1f,
    .embr_sleep = 0.05f,
    .embr_flip = 0.02f,
    .unknown = 0.8f,
    .noise = 0.03f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, UNKNOWN,
                "embr_classify_set_command should select UNKNOWN");
}

ZTEST(embr_classify, test_set_command_selects_noise_success) {
  embr_label_scores scores = {
    .embr_glow = 0.1f,
    .embr_sleep = 0.05f,
    .embr_flip = 0.02f,
    .unknown = 0.03f,
    .noise = 0.8f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, NOISE,
                "embr_classify_set_command should select NOISE");
}

ZTEST(embr_classify, test_set_command_tie_defaults_unknown_success) {
  embr_label_scores scores = {
    .embr_glow = 0.4f,
    .embr_sleep = 0.4f,
    .embr_flip = 0.1f,
    .unknown = 0.05f,
    .noise = 0.05f,
  };
  embr_command command = EMBR_RESET;

  int ret = embr_classify_set_command(scores, &command);
  zassert_equal(ret, 0, "embr_classify_set_command should return 0");
  zassert_equal(command, UNKNOWN,
                "embr_classify_set_command should default to UNKNOWN");
}

ZTEST(embr_classify, test_set_command_null_returns_einval_fail) {
  embr_label_scores scores = { 0 };

  int ret = embr_classify_set_command(scores, NULL);
  zassert_equal(ret, -EINVAL,
                "embr_classify_set_command should return -EINVAL");
}

ZTEST(embr_classify, test_command_to_str_reset_success) {
  zassert_true(strcmp(embr_classify_command_to_str(EMBR_RESET), "EMBR_RESET") ==
                   0,
               "embr_classify_command_to_str should convert EMBR_RESET");
}

ZTEST(embr_classify, test_command_to_str_glow_success) {
  zassert_true(strcmp(embr_classify_command_to_str(EMBR_GLOW), "EMBR_GLOW") ==
                   0,
               "embr_classify_command_to_str should convert EMBR_GLOW");
}

ZTEST(embr_classify, test_command_to_str_sleep_success) {
  zassert_true(strcmp(embr_classify_command_to_str(EMBR_SLEEP), "EMBR_SLEEP") ==
                   0,
               "embr_classify_command_to_str should convert EMBR_SLEEP");
}

ZTEST(embr_classify, test_command_to_str_flip_success) {
  zassert_true(strcmp(embr_classify_command_to_str(EMBR_FLIP), "EMBR_FLIP") ==
                   0,
               "embr_classify_command_to_str should convert EMBR_FLIP");
}

ZTEST(embr_classify, test_command_to_str_unknown_success) {
  zassert_true(strcmp(embr_classify_command_to_str(UNKNOWN), "UNKNOWN") == 0,
               "embr_classify_command_to_str should convert UNKNOWN");
}

ZTEST(embr_classify, test_command_to_str_noise_success) {
  zassert_true(strcmp(embr_classify_command_to_str(NOISE), "NOISE") == 0,
               "embr_classify_command_to_str should convert NOISE");
}

ZTEST(embr_classify, test_command_to_str_invalid_returns_invalid_fail) {
  zassert_true(strcmp(embr_classify_command_to_str((embr_command)999),
                      "INVALID") == 0,
               "embr_classify_command_to_str should return INVALID");
}
