# Error Reporting Architecture

This document describes how `embr` reports errors across ISR, driver, and
application layers without blocking.

## Goals

- **ISR-safe reporting** for callbacks and high-priority contexts.
- **Deferred handling** in thread context.
- **App-owned policy** for logging and escalation.

## Overview

`embr` uses an error ID mechanism for asynchronous reporting:

- **Producers** call `embr_error_report()` from ISR or callback contexts.
- **The error module** coalesces error IDs and schedules deferred work.
- **The app** registers a handler and performs logging or policy decisions.

## Reporting Flow

1. **Report**: A driver or callback calls `embr_error_report(ERROR_ID)`.
2. **Defer**: The module sets a pending bit and schedules work.
3. **Handle**: The work handler iterates all pending IDs.
4. **App policy**: The registered app handler logs and can later add counters,
   telemetry, or escalation.

## Current Usage

- **VM3011 callbacks** report PDM overflow and set-buffer errors.
- **LED work handler** reports toggle failures when the deferred LED-off
  toggle fails.

App-level errors that can be handled synchronously (for example init failures
or state-machine failures) are logged and returned directly instead of going
through deferred error IDs.

## Error ID Mapping

`embr_error_id_to_str()` translates error IDs into consistent, human-readable
messages. Centralizing this mapping keeps strings stable across the codebase and
avoids duplicated switch logic in the app.

## Runtime Details

- The deferred transport uses one global work item via `kernel_wrap_work_*`.
- `embr_error_report()` initializes that work item lazily on first use.
- Work submission is retried up to 3 times; repeated submission failure drops
  pending bits to avoid stale replay.
- Internal diagnostics are available through `embr_error_get_stats()`:
  `work_init_fail_count` and `work_submit_fail_count` (both saturating `uint8_t`).

## Notes

- Repeated IDs are coalesced per work cycle to avoid log floods.
- Reporting is safe for ISR/high-priority callbacks because it only sets bits
  and attempts deferred submission.
- If no handler is registered, deferred IDs are dropped (no logging). This does
  not affect direct synchronous app-layer logging.
