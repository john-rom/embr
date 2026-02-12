# Multithreading Architecture

This document describes the runtime threading model used by `embr`. It focuses on
how capture, inference, and application control flow interact.

## Overview

`embr` uses two primary execution contexts:

- **App thread (main)**: Runs the application state machine, blocks on
  semaphores, and orchestrates capture/recovery.
- **Edge Impulse thread**: The EI wrapper starts a dedicated worker thread that
  performs inference when a full window is ready.

The app thread and EI thread communicate via the EI wrapper’s internal buffer
and an internal semaphore that signals when enough data is available for
inference.

## App Thread Responsibilities

The app thread manages capture through an explicit state machine:

- **WAIT_WOS**: Block until the wake-on-sound (WOS) semaphore fires.
- **CAPTURE_SLICE**: Wait for a PDM buffer, discard the first non-empty startup
  buffer, then enqueue audio into the inference buffer.
- **WINDOW_CHECK**: Stop/reset the mic once the inference window is complete.
- **RECOVER**: On timeout policy trigger, deinit/reinit the mic and reset
  capture state.

The app thread does not run inference. It feeds data through `ei_wrap_add_data()`
and controls the capture lifecycle.

## Edge Impulse Thread Responsibilities

The EI wrapper’s worker thread performs inference asynchronously:

- Blocks on an internal EI semaphore until a full inference window is available.
- Runs the classifier for the current window.
- Invokes the result callback registered via `ei_wrap_init()`.

This allows capture to continue while inference is running, and keeps the app
thread focused on orchestration.

## Workqueue Context

In addition to the two primary threads, `embr` uses Zephyr workqueue context
for deferred callbacks:

- LED capture-end toggle is submitted as `k_work` from app flow.
- Error ID handling runs in deferred work via `kernel_wrap_work_submit()`.

These work handlers should stay short and non-blocking.

## Data Flow Summary

1. WOS triggers and releases the WOS semaphore.
2. The app thread starts DMIC capture and waits for PDM buffers.
3. Each buffer is copied into the EI input buffer via `ei_wrap_add_data()`.
4. When the window is complete, the EI thread runs the classifier.
5. The result callback processes classification output and restarts prediction
   as needed.

## Error Handling and Recovery

If PDM delivery stalls or malformed frame sizes persist, the app uses the same
timeout-driven recovery policy:

- Stop and deinit the mic
- Reinit the mic
- Reset capture state

After 3 consecutive timeout-policy events, the app escalates by returning
`-ETIMEDOUT` to `main`, which triggers a cold reboot.
