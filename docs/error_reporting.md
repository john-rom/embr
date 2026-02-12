# Error Reporting Architecture

This document describes how embr reports errors across ISR, driver, and
application layers without blocking or excessive latency.

## Goals

- **ISR-safe reporting** for callbacks and high-priority contexts.
- **Deferred handling** in thread context.
- **App-owned policy** for logging and escalation.

## Overview

embr uses an error ID mechanism for asynchronous reporting:

- **Producers** call `embr_error_report()` from ISR or callback contexts.
- **The error module** coalesces error IDs and schedules deferred work.
- **The app** registers a handler and performs logging or policy decisions.

## Reporting Flow

1. **Report**: A driver or callback calls `embr_error_report(ERROR_ID)`.
2. **Defer**: The error module sets a bit and schedules deferred work.
3. **Handle**: The work handler iterates all pending IDs.
4. **App policy**: The registered app handler logs and can later add counters,
   telemetry, or escalation.

## Current Usage

- **VM3011 callbacks** report PDM overflow and set-buffer errors.
- **LED work handler** reports toggle failures when the deferred LED-off
  toggle fails.

App-level errors that can be handled synchronously (e.g., init failures or
state machine errors) are logged and returned directly, rather than going
through the deferred error ID path.

## Error ID Mapping

`embr_error_id_to_str()` translates error IDs into consistent, human-readable
messages. Centralizing this mapping keeps strings stable across the codebase and
avoids duplicated switch logic in the app.

## Notes

- Repeated errors are **coalesced** per work cycle to avoid log floods.
- The system is safe for **ISR or high-priority callbacks** because reporting
  does not block or allocate.
- If no handler is registered, errors reported via `embr_error_report()` are
  dropped (no logging). This does not affect direct app-layer logging.
