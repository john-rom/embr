#ifndef EMBR_TYPES_H_
#define EMBR_TYPES_H_

/**
 * @file embr_types.h
 * @brief Shared types for classification and commands.
 */

/** Command selected from classifier output. */
typedef enum embr_command {
  EMBR_RESET,
  EMBR_GLOW,
  EMBR_SLEEP,
  EMBR_FLIP,
  UNKNOWN,
  NOISE
} embr_command;

/** Single classification result from the model. */
typedef struct embr_classification_results {
  const char *label;
  float probability;
  float anomaly;
} embr_classification_results;

/** Aggregated per-label scores for a full inference window. */
typedef struct embr_label_scores {
  float ember_glow;
  float ember_sleep;
  float ember_flip;
  float unknown;
  float noise;
} embr_label_scores;

#endif /* EMBR_TYPES_H_ */
