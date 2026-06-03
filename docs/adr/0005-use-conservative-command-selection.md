# 0005 - Use Conservative Command Selection

## Status

Accepted

## Context

Voice control should avoid unintended lighting actions. Classifier output can
include explicit command labels, `unknown`, and `noise`. Equal or ambiguous
scores should not be treated as a valid command.

## Decision

`embr` maps classifier labels into per-label scores and selects the command with
the highest score. `unknown` is the default command. If there is a tie for the
best score, the selected command is `UNKNOWN`.

`UNKNOWN` and `NOISE` are treated as successful no-op classifications by the
transport layer. `EMBR_RESET` and invalid command values are treated as invalid
transport inputs.

## Consequences

- Ambiguous classifier output does not trigger light changes.
- The transport layer does not need to know classifier score details.
- The app can log a classification result even when no transport action is
  sent.
- Thresholding can be added later, but the current MVP relies on highest-score
  selection plus tie-to-unknown behavior.

## Alternatives Considered

- **Always send the highest non-unknown command**: rejected because it can act
  on ambiguous or noisy classifications.
- **Use only a fixed confidence threshold**: deferred because it needs model
  calibration data and field testing.
- **Treat `NOISE` as an error**: rejected because noise is an expected model
  outcome, not a transport or application failure.
