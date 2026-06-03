# 0006 - Use Thread Multicast CoAP For MVP Command Transport

## Status

Accepted

## Context

`embr` needs to send small voice-triggered light commands across a local Thread
network. The command source is not intended to be a full Matter controller in
the MVP. Matter translation is handled by the companion `harth` bridge.

The transport must be simple enough to validate end-to-end while keeping the
client firmware focused on audio capture and inference.

## Decision

The MVP transport uses Thread multicast CoAP:

- UDP port `5683`.
- URI path `light`.
- Non-confirmable `PUT` requests.
- Single-byte ASCII payloads:
  - `'0'`: sleep/off
  - `'1'`: glow/on
  - `'2'`: flip/toggle
- Thread realm-local all-nodes multicast address `ff03::1`.

`harth` receives the CoAP command and translates it to Matter binding actions.

## Consequences

- The wire contract is small, observable, and easy to test.
- `embr` avoids pulling Matter controller behavior into the voice device.
- Multicast supports a simple local command broadcast path.
- Non-confirmable CoAP does not provide delivery acknowledgement.
- The bridge owns Matter-specific command dispatch and binding behavior.

## Alternatives Considered

- **Matter directly from `embr`**: rejected for the MVP because it would make the
  voice device responsible for Matter commissioning, binding, and controller
  behavior.
- **Unicast CoAP pairing**: deferred because multicast is sufficient for the
  current local bridge path.
- **MQTT or Wi-Fi transport**: rejected because the product goal is Thread-local
  control without depending on Wi-Fi or cloud infrastructure.
