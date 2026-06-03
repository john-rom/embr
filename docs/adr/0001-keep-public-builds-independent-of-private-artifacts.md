# 0001 - Keep Public Builds Independent Of Private Artifacts

## Status

Accepted

## Context

`embr` is intended to be a public portfolio-quality firmware repository while
the real Edge Impulse deployment and Thread network credentials remain private.
The repository still needs to build in CI and remain useful to readers without
requiring private model files, datasets, or local network secrets.

The application also needs a stable inference API so the real model can be
enabled locally without changing application logic.

## Decision

The default public build uses a stub `ei_wrap` backend. Real Edge
Impulse-backed builds are enabled only through local configuration in
`prj.local.conf` and a local deployment under `third_party/edge_impulse/embr`.

Thread network credentials are also kept out of tracked configuration and are
provided through local config overrides.

## Consequences

- Public CI can build the firmware without private artifacts.
- The app-level inference, classification, and transport code remains testable
  against a stable wrapper contract.
- Local real-model builds can use the same application code path by enabling
  `CONFIG_EDGE_IMPULSE`.
- The public default firmware emits deterministic `unknown` inference results
  and does not represent real voice-command behavior.
- Documentation must clearly distinguish public/default builds from local
  real-model builds.

## Alternatives Considered

- **Commit the real Edge Impulse deployment**: rejected because model artifacts
  are private and large enough to make public maintenance worse.
- **Disable public firmware builds**: rejected because buildable public code is
  a core repository goal.
- **Stub at a higher application layer**: rejected because it would leave the
  inference wrapper boundary less exercised.
