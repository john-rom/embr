#ifndef EMBR_CLASSIFY_H_
#define EMBR_CLASSIFY_H_

/**
 * @file embr_classify.h
 * @brief Classification helpers for Edge Impulse output.
 *
 * Provides routines to read classifier results, map label scores,
 * and select an embr command.
 */

#include "embr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read the next classification result.
 *
 * @param results Pointer to result storage
 * @return 0 on success, -ENOENT when no more results, or -errno on failure
 */
int embr_classify_get_next_result(embr_classification_results *results);

/**
 * @brief Assign a result's probability to the matching label score.
 *
 * Maps a classifier result label to its score field.
 *
 * @param results Pointer to inference result
 * @param scores Pointer to label score storage
 * @return 0 on success, or -errno on failure
 */
int embr_classify_assign_label_scores(
    const embr_classification_results *results, embr_label_scores *scores);

/**
 * @brief Retrieve all results and populate label scores.
 *
 * @param results Pointer to inference result storage
 * @param embr_label_scores Pointer to label score storage
 * @return 0 on success, or -errno on failure
 */
int embr_classify_get_all_results(embr_classification_results *results,
                                  embr_label_scores *embr_label_scores);

/**
 * @brief Select the dominant command from label scores.
 *
 * @param scores Label scores
 * @param command Pointer to command storage
 * @return 0 on success, or -errno on failure
 */
int embr_classify_set_command(embr_label_scores scores, embr_command *command);

/**
 * @brief Convert a command to a string label.
 *
 * @param command Command to convert
 * @return String label for the command
 */
const char *embr_classify_command_to_str(embr_command command);

#ifdef __cplusplus
}
#endif

#endif /* EMBR_CLASSIFY_H_ */
