# 0003 - Model Capture As An Explicit Application State Machine

## Status

Accepted

## Context

The voice pipeline spans WOS callbacks, PDM buffer delivery, microphone
lifecycle, Edge Impulse buffering, and recovery from timeout or malformed-frame
conditions. Encoding those transitions only through nested callbacks or
scattered flags would make failures difficult to reason about.

## Decision

The application models capture with an explicit state machine:

- `WAIT_WOS`: wait for wake-on-sound.
- `CAPTURE_SLICE`: wait for and process one PDM slice.
- `WINDOW_CHECK`: decide whether the inference window is complete.
- `RECOVER`: stop, deinit, reinit, and reset capture state after recovery-class
  failures.

State-transition logic that can be tested without hardware lives in
`embr_app_logic`.

## Consequences

- Capture behavior is easier to test at the app contract boundary.
- Recovery behavior is visible and deterministic.
- The app thread remains the owner of capture orchestration.
- The state machine adds a small amount of boilerplate, but reduces hidden
  lifecycle coupling.
- New capture states should be added only when they represent a distinct runtime
  responsibility.

## Alternatives Considered

- **Callback-only orchestration**: rejected because it would spread product
  policy across driver callbacks and inference callbacks.
- **Single loop with implicit flags**: rejected because it would obscure
  recovery and window-completion decisions.
- **Hardware-level recovery only**: rejected because the app needs product-level
  escalation policy for repeated timeouts.
