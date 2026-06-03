# 0008 - Separate Command Policy From Transport Backend Mechanics

## Status

Accepted

## Context

The application command model is not the same as the wire protocol. The app
knows about classifier commands such as `EMBR_GLOW`, `EMBR_SLEEP`, `EMBR_FLIP`,
`UNKNOWN`, and `NOISE`. The CoAP backend knows about URI paths, payload bytes,
OpenThread attachment, and send mechanics.

Coupling these directly would make command policy harder to test and would
spread networking details into classification code.

## Decision

`embr_transport` owns product-level command policy:

- Actionable app commands are mapped to CoAP light commands.
- `UNKNOWN` and `NOISE` are ignored as successful no-ops.
- `EMBR_RESET` and invalid commands are rejected.

`embr_coap_client` owns the SDK-independent CoAP command API. The
OpenThread/NCS mechanics are isolated behind `openthread_coap_wrap`.

## Consequences

- Classification, transport policy, and OpenThread mechanics can be tested at
  separate boundaries.
- The wire contract is concentrated in `embr_coap_protocol`.
- A future transport backend can replace CoAP mechanics without changing
  classifier command selection.
- Transport initialization remains an explicit app startup dependency.

## Alternatives Considered

- **Send CoAP directly from the inference callback**: rejected because it would
  couple classifier results to networking details.
- **Put all transport logic in the platform layer**: rejected because command
  policy is application behavior, not hardware abstraction.
- **Expose OpenThread APIs to tests**: rejected because unit tests should verify
  the module contract without requiring vendor networking headers.
