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

  if (!strcmp(results->label, "ember_glow")) {
    scores->ember_glow = results->probability;
  } else if (!strcmp(results->label, "ember_sleep")) {
    scores->ember_sleep = results->probability;
  } else if (!strcmp(results->label, "ember_flip")) {
    scores->ember_flip = results->probability;
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

  if (scores.ember_glow > scores.ember_sleep &&
      scores.ember_glow > scores.ember_flip &&
      scores.ember_glow > scores.unknown && scores.ember_glow > scores.noise) {
    *command = EMBR_GLOW;
  } else if (scores.ember_sleep > scores.ember_glow &&
             scores.ember_sleep > scores.ember_flip &&
             scores.ember_sleep > scores.unknown &&
             scores.ember_sleep > scores.noise) {
    *command = EMBR_SLEEP;
  } else if (scores.ember_flip > scores.ember_glow &&
             scores.ember_flip > scores.ember_sleep &&
             scores.ember_flip > scores.unknown &&
             scores.ember_flip > scores.noise) {
    *command = EMBR_FLIP;
  } else if (scores.unknown > scores.ember_glow &&
             scores.unknown > scores.ember_sleep &&
             scores.unknown > scores.ember_flip &&
             scores.unknown > scores.noise) {
    *command = UNKNOWN;
  } else if (scores.noise > scores.ember_glow &&
             scores.noise > scores.ember_sleep &&
             scores.noise > scores.ember_flip &&
             scores.noise > scores.unknown) {
    *command = NOISE;
  } else {
    *command = UNKNOWN;
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
