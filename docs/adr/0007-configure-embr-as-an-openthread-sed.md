# 0007 - Configure embr As An OpenThread SED

## Status

Accepted

## Context

`embr` is a leaf command source. It sends voice-triggered commands and does not
route Thread traffic for other nodes. During integration, router-capable and
non-sleepy Thread configurations increased idle activity and interfered with
the expected wake-on-sound runtime behavior.

The device does not expect application-level inbound messages during normal
operation.

## Decision

`embr` is configured as an OpenThread Minimal Thread Device / Sleepy End Device
(MTD/SED). The tracked `prj.conf` owns this product role. Thread network
credentials remain local.

With NCS v2.4.0, the default SED poll period resolves to `236000` ms. `embr`
uses that default because inbound application traffic is not part of the normal
MVP flow.

## Consequences

- The Thread role matches the product role: a sleepy leaf command source.
- Wake-on-sound idle behavior remains reliable while Thread is enabled.
- The device does not route traffic for the mesh.
- Inbound message latency is governed by sleepy-device polling behavior.
- If future features require responsive inbound control, the poll period or
  device role must be revisited.

## Alternatives Considered

- **Full Thread Device (FTD)**: rejected because routing behavior is unnecessary
  and increased runtime activity for this product role.
- **Minimal Thread Device without sleepy behavior**: rejected after testing
  showed elevated activity without restoring the desired WOS idle behavior.
- **Custom shorter poll period**: deferred because normal operation does not
  require application-level inbound messages.
