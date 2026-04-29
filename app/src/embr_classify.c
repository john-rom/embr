#include "embr_classify.h"

#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <zephyr/sys/util.h>

#include "ei_wrap.h"

__weak int embr_classify_get_next_result(embr_classification_results *results) {
  if (results == NULL) {
    return -EINVAL;
  }

  return ei_wrap_get_next_classification_result(&results->label,
                                                &results->probability, NULL);
}

__weak int
embr_classify_assign_label_scores(const embr_classification_results *results,
                                  embr_label_scores *scores) {
  if (results == NULL || scores == NULL) {
    return -EINVAL;
  }

  if (!strcmp(results->label, "embr_glow")) {
    scores->embr_glow = results->probability;
  } else if (!strcmp(results->label, "embr_sleep")) {
    scores->embr_sleep = results->probability;
  } else if (!strcmp(results->label, "embr_flip")) {
    scores->embr_flip = results->probability;
  } else if (!strcmp(results->label, "unknown")) {
    scores->unknown = results->probability;
  } else if (!strcmp(results->label, "noise")) {
    scores->noise = results->probability;
  } else {
    return -ENOENT;
  }

  return 0;
}

int embr_classify_get_all_results(embr_classification_results *results,
                                  embr_label_scores *scores) {
  int err = 0;
  if (scores != NULL) {
    memset(scores, 0, sizeof(*scores));
  }
  while (true) {
    err = embr_classify_get_next_result(results);
    if (err) {
      if (err == -ENOENT) {
        err = 0;
      }
      break;
    }

    err = embr_classify_assign_label_scores(results, scores);
    if (err) {
      return err;
    }
  }

  return err;
}

int embr_classify_set_command(embr_label_scores scores, embr_command *command) {
  if (command == NULL) {
    return -EINVAL;
  }

  struct score_candidate {
    float score;
    embr_command command;
  };
  const struct score_candidate candidates[] = {
      {scores.embr_glow, EMBR_GLOW}, {scores.embr_sleep, EMBR_SLEEP},
      {scores.embr_flip, EMBR_FLIP}, {scores.unknown, UNKNOWN},
      {scores.noise, NOISE},
  };
  float best_score = scores.unknown;
  embr_command best_command = UNKNOWN;
  bool has_tie_for_best = false;
  size_t i;

  for (i = 0; i < ARRAY_SIZE(candidates); i++) {
    if (candidates[i].score > best_score) {
      best_score = candidates[i].score;
      best_command = candidates[i].command;
      has_tie_for_best = false;
    } else if (candidates[i].command != UNKNOWN &&
               candidates[i].score == best_score) {
      has_tie_for_best = true;
    }
  }

  if (has_tie_for_best) {
    *command = UNKNOWN;
  } else {
    *command = best_command;
  }
  return 0;
}

const char *embr_classify_command_to_str(embr_command command) {
  switch (command) {
  case EMBR_RESET:
    return "EMBR_RESET";
  case EMBR_GLOW:
    return "EMBR_GLOW";
  case EMBR_SLEEP:
    return "EMBR_SLEEP";
  case EMBR_FLIP:
    return "EMBR_FLIP";
  case UNKNOWN:
    return "UNKNOWN";
  case NOISE:
    return "NOISE";
  default:
    return "INVALID";
  }
}
