# 0002 - Use Wake-On-Sound Gated Audio Capture

## Status

Accepted

## Context

`embr` is a battery-oriented voice-control device. Continuous audio capture is
simple, but it keeps the DMIC path active and prevents the product from reaching
the intended low-power idle behavior.

The VM3011 microphone supports wake-on-sound (WOS), which allows the firmware
to remain idle until the microphone reports acoustic activity.

## Decision

The application uses WOS as the gate for active audio capture. The app thread
blocks on the WOS semaphore, starts DMIC capture only after WOS fires, feeds a
bounded inference window, then stops and resets the microphone before returning
to idle.

## Consequences

- Idle current is driven by the armed WOS path rather than continuous DMIC
  capture.
- Capture latency includes WOS detection and DMIC startup behavior.
- The app must manage microphone lifecycle explicitly across start, stop,
  reset, deinit, and recovery paths.
- Power profiling must measure both idle and event windows, because average
  current depends heavily on wake frequency.

## Alternatives Considered

- **Continuous capture**: rejected because it is easier to implement but does
  not meet the low-power product goal.
- **Application-level polling without WOS**: rejected because it keeps more
  firmware and peripheral activity in the idle path.
- **Always-on inference**: rejected because it increases power draw and ties
  product behavior to a much heavier runtime model.
