# 0009 - Use Deferred Error ID Reporting

## Status

Accepted

## Context

Some errors originate from callbacks or high-priority contexts, including
VM3011 PDM overflow and buffer errors. These paths should not perform expensive
logging or app-level policy handling directly.

The app still needs a consistent way to observe asynchronous driver and
callback failures.

## Decision

`embr` uses deferred error ID reporting:

- Producers call `embr_error_report(EMBR_ERR_ID_*)`.
- The error module sets an atomic pending bit.
- A singleton work item handles pending IDs in thread context.
- The app registers the policy handler that logs or escalates each ID.

Repeated IDs are coalesced per work cycle. Work submission is retried for a
bounded number of attempts, and stale pending bits are dropped if scheduling
continues to fail.

## Consequences

- Callback and high-priority paths stay short.
- Error strings and handler policy are centralized.
- Repeated identical errors do not flood the log in a single work cycle.
- The mechanism reports error categories, not every individual occurrence.
- If no handler is registered, deferred IDs are dropped.

## Alternatives Considered

- **Log directly from every producer**: rejected because it couples callback
  paths to logging policy and can do too much work in sensitive contexts.
- **Queue every error occurrence**: rejected for the MVP because error category
  observability is sufficient and bounded state is simpler.
- **Reset immediately on callback errors**: rejected because recovery and
  escalation policy belongs at the app layer.
