# 0010 - Separate Application Policy From Platform Mechanics

## Status

Accepted

## Context

`embr` depends on Zephyr, Nordic SDK APIs, VM3011 driver behavior, Edge Impulse
integration, and OpenThread networking. Directly mixing product decisions with
vendor and hardware mechanics would make the firmware harder to test and harder
to migrate from prototype code.

## Decision

The repository keeps a clear ownership boundary:

- `app/` owns product behavior, command policy, inference orchestration,
  logging decisions, and transport policy.
- `platform/` owns hardware-facing modules and thin wrappers around Zephyr,
  drivers, or SDK APIs.
- Shared wire contracts are kept small and SDK-independent.
- Unit tests mock dependencies at module boundaries rather than pulling in full
  vendor stacks.

Wrappers are added when they protect a meaningful boundary: hardware access,
vendor SDK mechanics, or shared infrastructure. They are not added only for
style or abstraction symmetry.

## Consequences

- Product behavior can be tested without connected hardware.
- Vendor and board-specific details are concentrated in platform modules.
- The codebase remains easier to explain as a portfolio project.
- Thin wrappers need discipline: tests should verify contract behavior, not
  incidental internal sequencing.
- New modules must declare which layer owns their policy before implementation.

## Alternatives Considered

- **Direct vendor calls throughout app code**: rejected because it would make
  unit testing and future migration harder.
- **Wrap every Zephyr API unconditionally**: rejected because it adds noise and
  creates fake abstraction where no boundary exists.
- **Put all behavior under platform modules**: rejected because product policy
  belongs in application code.
