# 0004 - Use A Fixed Edge Impulse Window And Label Contract

## Status

Accepted

## Context

The audio path, Edge Impulse model, inference timing, and power profile must
agree on a shared set of timing and label assumptions. If the model changes its
labels or windowing parameters without matching firmware changes, the
application can misclassify commands or fail to complete inference windows.

## Decision

`embr` uses a fixed model contract:

- Audio input is `16000 Hz`.
- Each inference window is `1000 ms`.
- Each PDM slice is `4000` samples.
- A full inference window is `4` slices.
- The model emits `embr_glow`, `embr_sleep`, `embr_flip`, `unknown`, and
  `noise`.

The app discards the first non-empty startup slice after WOS to avoid stale data
and startup transient noise. Persistent malformed frame sizes reuse the timeout
recovery path.

## Consequences

- Firmware timing, model configuration, and power profiling are aligned.
- The app can use simple fixed-size buffering for the current MVP.
- A future model with different labels, sample rate, or window size requires a
  coordinated firmware update.
- Startup-slice discard slightly extends each wake event but avoids feeding
  stale or transient data into inference.

## Alternatives Considered

- **Infer timing from the model at runtime**: rejected for the MVP because the
  app already controls the embedded capture window and needs deterministic
  buffer sizing.
- **Accept arbitrary model labels**: rejected because command mapping must be
  explicit and safe.
- **Feed the first startup buffer**: rejected because VM3011/DMIC startup can
  produce stale or transient data.
