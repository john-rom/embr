# Multithreading Architecture

This document describes the runtime threading model used by `embr`. It focuses on
how capture, inference, and application control flow interact.

## Overview

`embr` uses several runtime execution contexts. The two central contexts for the
voice pipeline are:

- **App thread (main)**: Runs the application state machine, blocks on
  semaphores, and orchestrates capture/recovery.
- **Inference backend context**: The real Edge Impulse backend uses a worker
  thread to perform inference when a full window is ready. The public stub
  backend invokes the result callback synchronously when the simulated window
  fills.

The app thread and real Edge Impulse backend communicate via the EI wrapper's
internal buffer and an internal semaphore that signals when enough data is
available for inference.

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

## Inference Backend Responsibilities

The real EI wrapper's worker thread performs inference asynchronously:

- Blocks on an internal EI semaphore until a full inference window is available.
- Runs the classifier for the current window.
- Invokes the result callback registered via `ei_wrap_init()`.

The app thread owns capture orchestration. Once the inference window is full,
the app stops and resets the mic, returns to WOS-gated idle behavior, and lets
the result callback handle command selection and prediction restart.

## Networking Context

OpenThread owns Thread attachment callbacks and lower-level networking work.
`embr` initializes transport during app startup, tracks Thread attachment in the
OpenThread CoAP wrapper, and sends multicast CoAP commands from the inference
result callback when an actionable command is selected.

The transport path should not block capture-state progress beyond the immediate
send call. If Thread is detached, the transport returns an error and the app
restarts prediction.

## Workqueue Context

In addition to the two primary threads, `embr` uses Zephyr workqueue context
for deferred callbacks:

- LED capture-end toggle is submitted as `k_work` from app flow.
- Error ID handling runs in deferred work via
  `kernel_wrap_error_work_submit()`.

These work handlers should stay short and non-blocking.

## Data Flow Summary

1. WOS triggers and releases the WOS semaphore.
2. The app thread starts DMIC capture and waits for PDM buffers.
3. Each buffer is copied into the EI input buffer via `ei_wrap_add_data()`.
4. When the window is complete, the app stops/resets the mic and returns to
   WOS-gated idle behavior.
5. The inference backend runs the classifier for the completed window.
6. The result callback processes classification output, sends actionable
   commands through Thread/CoAP, and restarts prediction as needed.

## Error Handling and Recovery

If PDM delivery stalls or malformed frame sizes persist, the app uses the same
timeout-driven recovery policy:

- Stop and deinit the mic
- Reinit the mic
- Reset capture state

After 3 consecutive timeout-policy events, the app escalates by returning
`-ETIMEDOUT` to `main`, which triggers a cold reboot.
