# Edge Impulse Compatibility

This repository's public/default build uses a stub inference backend. Local
real Edge Impulse builds are supported through `prj.local.conf` and a
compatible deployment under `third_party/edge_impulse/embr`.

## Required Labels

A compatible classification model must emit these exact labels:

- `embr_glow`
- `embr_sleep`
- `embr_flip`
- `unknown`
- `noise`

## Required Audio Parameters

The app is currently aligned to:

- `16000 Hz` audio input
- `1000 ms` inference window
- `250 ms` window increase / stride

In repo terms, this means:

- `16000` samples per inference window
- `4000` samples per slice
- `4` slices per inference window

If your model uses different labels or windowing parameters, update the app
code to match before using it.
