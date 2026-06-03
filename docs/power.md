# Power Report
This document contains power profiling results for **embr**, captured with the **Nordic Power Profiler Kit II (PPK2)**.
Values are intended to provide a repeatable baseline and enable apples-to-apples comparison as features are added
(e.g., DMIC bring-up, wake-on-sound (WOS) + DSP/inference bursts, Thread/Matter activity).

---

## Measurement Setup
<figure>
  <img src="assets/power/t53_ppk2.png" width="600" alt="Thingy:53 connected to the PPK2">
  <figcaption><em>Thingy:53 connected to the PPK2 via the current measurement and debug board.</em></figcaption>
</figure>
</p>

**Tool**: Nordic Power Profiler Kit II (PPK2)  
**Measurement mode**: Ammeter mode (target powered through PPK2)  
**Supply voltage**: 3.12 V (measured at Thingy:53 VIN with DMM; PPK2 VOUT → VIN)  
**Target hardware**: Nordic Thingy:53  
**Latest firmware state**: WOS capture + DSP/inference + Thread-connected CoAP command transmission  
**Logging**: RTT

**Notes**
- Capture windows are taken after initialization settles unless stated otherwise.

---

## Interpretation
The measurements below currently establish:
- A general baseline for:
  - Verifying the system reaches low-power idle between specific events (e.g LED blink, WOS + DSP/inference events)
  - Comparing incremental power cost as features are added
  - Identifying unexpected background activity early

- DMIC audio capture baseline:
  - 16 kHz sample rate
  - Continuous capture, no sleep/idle
 
- DMIC WOS event baseline:
  - 1 s bursts, 16 k samples
  - Idle preceding/following burst activity
 
- WOS bursts with subsequent DSP/inference baseline:
  - 1 s capture events, 16 k samples
  - ~175 ms avg. DSP + inference time
  - Idle preceding/following burst activity

- Thread networking and CoAP command baseline:
  - OpenThread MTD/SED command-source role
  - Network-unavailable attach retry behavior (Thread stack up, no network reachable)
  - Thread-connected idle with WOS + DSP/inference armed
  - Voice-triggered WOS capture, DSP/inference, and multicast CoAP command transmission in the same event path

Future profiles planned:
- Longer-duration Thread/SED connected idle
- Thread commissioning / join transient
- Isolated radio TX/RX bursts
- End-to-end voice-triggered lighting control path through the Matter boundary

---

## Results Summary
**Notes**
- Results are presented in reverse chronological order. Measurements for the latest firmware/feature set will appear first.
- Trial count and selection window are listed per section.
- Steady-state captures use longer post-init windows.
- Event measurements use selected regions around the active behavior being profiled.
- Primary measurements:
  -  Average current and average peak current
  -  Measurements from DMIC bring-up and later also include: min/max, range, standard deviation (std dev), and coefficient of variation (CV)
 
### ~~~ Thread Networking + CoAP — Attach Retry, Connected Idle, and WOS Command Event ~~~
**Behavior**
- Thread stack enabled as an OpenThread MTD/SED command source
- When no Thread network is available, the device repeatedly attempts to attach instead of reaching the connected idle floor
- When the Thread network is available, the device attaches, idles with WOS + DSP/inference armed, and sends voice-triggered multicast CoAP light commands
- WOS command event path: WOS audio capture → DSP/inference → CoAP command transmission → return to Thread-connected idle

**Notes**
- No-network attach retry trials: 3; 24.80-25.59 s per trial
- Thread-connected idle trials: 3; 20.01-26.63 s per trial
- WOS command event samples: 3 events from one longer Thread-connected run
- Event sub-selections:
  - WOS audio capture: ~1.003 s average selection window
  - DSP/inference + CoAP transmission: ~217.8 ms average selection window
  - Full WOS-to-CoAP command window: ~1.218 s average selection window
- Full command-window current and charge are taken from the direct PPK2 selection spanning WOS capture through CoAP transmission.
- The no-network captures are not an idle power floor; they represent repeated Thread attach attempts when no network is reachable.
- Figures below show one representative screenshot per profiled behavior/sub-selection; all trial screenshots are used for the calculations but are not embedded.
- PPK2 screenshot readouts are rounded to the displayed precision; derived statistics inherit that precision.

**Thread network unavailable (attach retry)**
- Average current: 3.97 mA
  - Min/Max: 3.91 mA / 4.07 mA
  - Range: 0.16 mA
  - Std dev (sample): 0.09 mA
  - CV: 2.26%

- Average peak current (attach-retry bursts): 25.50 mA
  - Min/Max: 21.86 mA / 28.18 mA
  - Range: 6.32 mA
  - Std dev (sample): 3.27 mA
  - CV: 12.81%

- Average charge per attach-retry window: 100.40 mC
  - Min/Max: 100.09 mC / 100.93 mC
  - Range: 0.84 mC
  - Std dev (sample): 0.46 mC
  - CV: 0.46%

**Thread-connected idle (WOS + DSP/inference armed)**
- Average current: 113.98 µA
  - Min/Max: 113.84 µA / 114.09 µA
  - Range: 0.25 µA
  - Std dev (sample): 0.13 µA
  - CV: 0.11%

- Average peak current (background spikes): 10.22 mA
  - Min/Max: 8.56 mA / 11.13 mA
  - Range: 2.57 mA
  - Std dev (sample): 1.44 mA
  - CV: 14.11%

**WOS audio capture event (Thread connected)**
- Average current: 713.67 µA
  - Min/Max: 708.22 µA / 719.07 µA
  - Range: 10.85 µA
  - Std dev (sample): 5.43 µA
  - CV: 0.76%

- Average peak current: 14.68 mA
  - Min/Max: 13.98 mA / 15.06 mA
  - Range: 1.08 mA
  - Std dev (sample): 0.61 mA
  - CV: 4.13%

- Average charge per WOS capture: 715.95 µC
  - Min/Max: 710.84 µC / 720.10 µC
  - Range: 9.26 µC
  - Std dev (sample): 4.70 µC
  - CV: 0.66%

**DSP/inference + CoAP transmission event**
- Average current: 3.34 mA
  - Min/Max: 3.28 mA / 3.40 mA
  - Range: 0.12 mA
  - Std dev (sample): 0.06 mA
  - CV: 1.80%

- Average peak current: 13.34 mA
  - Min/Max: 12.87 mA / 13.91 mA
  - Range: 1.04 mA
  - Std dev (sample): 0.53 mA
  - CV: 3.95%

- Average charge per DSP/inference + CoAP transmission: 726.97 µC
  - Min/Max: 708.57 µC / 742.19 µC
  - Range: 33.62 µC
  - Std dev (sample): 17.04 µC
  - CV: 2.34%

**Full WOS-to-CoAP command event**
- Average current: 1.187 mA
  - Min/Max: 1.18 mA / 1.19 mA
  - Range: 10.00 µA
  - Std dev (sample): 5.77 µA
  - CV: 0.49%

- Average peak current: 14.68 mA
  - Min/Max: 13.98 mA / 15.06 mA
  - Range: 1.08 mA
  - Std dev (sample): 0.61 mA
  - CV: 4.13%

- Average charge per WOS-to-CoAP command: 1.443 mC
  - Min/Max: 1.43 mC / 1.45 mC
  - Range: 20.00 µC
  - Std dev (sample): 11.55 µC
  - CV: 0.80%

**Derived comparisons against previous WOS + DSP/inference profile**
- Thread-connected idle current is 17.74 µA above the previous WOS + DSP/inference armed idle baseline (113.98 µA vs 96.24 µA).
- Network-unavailable attach retry averages 3.85 mA above Thread-connected idle (~34.8x connected idle current).
- Full WOS-to-CoAP command charge is ~86.8 µC above the previous WOS + DSP/inference event charge (1.443 mC vs 1.3565 mC), over a direct selection window ~43 ms longer (1.218 s vs ~1.175 s).
- These deltas include the full Thread-enabled runtime path and are not isolated radio-only TX/RX costs.

**Representative attach-retry capture (Thread network unavailable)**
<figure>
  <img src="assets/power/embr_thread_no_network_idle_1.png" width="1200" alt="Thread stack enabled with no Thread network available, attach retry current">
  <figcaption><em>Representative attach-retry capture with no Thread network available; repeated attach attempts dominate the window.</em></figcaption>
</figure>
</p>

**Representative steady-state capture (Thread connected idle)**
<figure>
  <img src="assets/power/embr_thread_connected_idle_1.png" width="1200" alt="Thread connected idle with WOS and DSP/inference armed">
  <figcaption><em>Representative Thread-connected idle capture with WOS + DSP/inference armed.</em></figcaption>
</figure>
</p>

**Representative event selections (one screenshot per sub-selection)**
<figure>
  <img src="assets/power/embr_thread_wos_capture_1.png" width="1200" alt="Thread-connected WOS audio capture selection showing approximately 1 second active capture">
  <figcaption><em>Representative Thread-connected WOS audio capture selection showing an approximately 1 s active capture window.</em></figcaption>
</figure>
</p>

<figure>
  <img src="assets/power/embr_thread_dsp_inference_coap_1.png" width="1200" alt="Thread-connected DSP/inference and CoAP command transmission selection">
  <figcaption><em>Representative Thread-connected DSP/inference + CoAP command transmission selection.</em></figcaption>
</figure>
</p>

<figure>
  <img src="assets/power/embr_thread_command_1.png" width="1200" alt="Full Thread-connected WOS-to-CoAP command selection">
  <figcaption><em>Representative full Thread-connected WOS-to-CoAP command selection spanning capture through command transmission.</em></figcaption>
</figure>
</p>

### ~~~ DSP/Inference after DMIC Wake-on-Sound (WOS) — Idle + Wake/Inference Event ~~~
**Behavior**
- Wake-on-sound (WOS) armed; device remains in low-power idle until a wake event occurs
- Wake event produces an ~1 s active capture window, then performs DSP and runs inference
- Returns to idle state

**Notes**
- Idle trials: 3; 26.85 s per trial
- Wake + DSP/inference event samples: 3 events, ~1.175 s average combined selection window per event
- Combined event average current is calculated from selected charge and time:
  - I_avg = (Q_capture + Q_DSP/inference) / (t_capture + t_DSP/inference)
- Combined event peak current is the larger peak observed across the capture and DSP/inference selections for that event

**Idle current (WOS + DSP/Inference armed)**
- Average current: 96.24 µA
  - Min/Max: 95.33 µA / 96.79 µA
  - Range: 1.46 µA
  - Std dev (sample): 0.80 µA
  - CV: 0.83%

- Average peak current (background spikes): 12.83 mA
  - Min/Max: 11.40 mA / 14.34 mA
  - Range: 2.94 mA
  - Std dev (sample): 1.47 mA
  - CV: 11.48%

**Wake + DSP/Inference event**
- Average current: 1.154 mA
  - Min/Max: 1.149 mA / 1.159 mA
  - Range: 9.75 µA
  - Std dev (sample): 4.89 µA
  - CV: 0.42%

- Average peak current: 16.34 mA
  - Min/Max: 15.73 mA / 17.30 mA
  - Range: 1.57 mA
  - Std dev (sample): 0.84 mA
  - CV: 5.16%

- Average charge per wake + DSP/inference event: 1356.53 µC
  - Min/Max: 1351.54 µC / 1362.08 µC
  - Range: 10.54 µC
  - Std dev (sample): 5.29 µC
  - CV: 0.39%

**Representative steady-state capture (idle)**
<figure>
  <img src="assets/power/embr_wos_inference_idle1.png" width="1200" alt="WOS + DSP/inference armed, idle current (no wake events)">
  <figcaption><em>WOS + DSP/inference armed, idle current (no wake events).</em></figcaption>
</figure>
</p>

**Representative event selections (wake + capture/DSP/inference events)**
<figure>
  <img src="assets/power/embr_wos_inference_ev1_capture.png" width="1200" alt="WOS + capture event (selected region) showing ~1 s active capture window">
  <figcaption><em>WOS + capture event (selected region) showing ~1 s active capture window.</em></figcaption>
</figure>
</p>

<figure>
  <img src="assets/power/embr_wos_inference_ev1_inference.png" width="1200" alt="DSP/Inference event (selected region) showing ~176 ms active inference window">
  <figcaption><em>DSP/Inference event (selected region) showing ~176 ms active DSP/inference window.</em></figcaption>
</figure>
</p>
 
### ~~~ DMIC Wake-on-Sound (WOS) — Idle + Wake Event ~~~
**Behavior**
- Wake-on-sound (WOS) armed; device remains in low-power idle until a wake event occurs
- Wake event produces an ~1 s active window, then returns to idle

**Notes**
- Idle trials: 3; 26.85 s per trial
- Wake event samples: 6 events, ~1.0 s selection window per event

**Idle current (WOS armed)**
- Average current: 101.44 µA
  - Min/Max: 98.08 µA / 105.75 µA
  - Range: 7.67 µA
  - Std dev (sample): 3.92 µA
  - CV: 3.87%

- Average peak current (background spikes): 11.33 mA
  - Min/Max: 10.82 mA / 11.63 mA
  - Range: 0.81 mA
  - Std dev (sample): 0.44 mA
  - CV: 3.92%

**Wake event (selection window ~1.006 s)**
- Average current: 673.25 µA
  - Min/Max: 666.52 µA / 677.54 µA
  - Range: 11.02 µA
  - Std dev (sample): 4.63 µA
  - CV: 0.69%

- Average peak current (wake burst): 14.83 mA
  - Min/Max: 12.45 mA / 16.19 mA
  - Range: 3.74 mA
  - Std dev (sample): 1.36 mA
  - CV: 9.14%

- Average charge per wake (selection): 677.66 µC
  - Min/Max: 672.72 µC / 681.65 µC
  - Range: 8.93 µC
  - Std dev (sample): 3.93 µC
  - CV: 0.58%

**Representative steady-state capture (idle)**
<figure>
  <img src="assets/power/embr_mic_wos_idle_1.png" width="1200" alt="WOS armed, idle current (no wake events)">
  <figcaption><em>WOS armed idle current (no wake events).</em></figcaption>
</figure>
</p>

**Representative steady-state capture (wake events)**
<figure>
  <img src="assets/power/embr_mic_wos_event_1.png" width="1200" alt="WOS events showing ~1 s active window per wake">
  <figcaption><em>WOS events showing ~1 s active window per wake (selection window used for per-event stats).</em></figcaption>
</figure>
</p>

### ~~~ DMIC (PDM) Bring-up - Continuous Audio Capture ~~~
**Behavior**
- Audio sample rate: 16kHz
- Mode: Mono, single channel
- PDM buffer size: 4000 samples
- Buffer release rate: 4 releases per second
- Peripherals enabled: DMIC

**Steady-state current (post init)**
- Average current: 646.08 µA
  - Min/Max: 644.98 µA / 647.29 µA
  - Range: 2.31 µA
  - Std dev (sample): 1.16 µA
  - CV: 0.18%
- Average peak current: 16.70 mA
  - Min/Max: 16.37 mA / 17.31 mA
  - Range: 0.94 mA
  - Std dev (sample): 0.53 mA
  - CV: 3.18%

**Representative steady-state capture**
<figure>
  <img src="assets/power/embr_mic_continuous_steady_1.png" width="1200" alt="DMIC running continuous capture at 16 kHz">
  <figcaption><em>DMIC running continuous audio capture at 16 kHz</em></figcaption>
</figure>
</p> 

### ~~~ Baseline — Blinky ~~~
**Behavior**
- Blink period: 500 ms  
- Duty cycle: 50%
- Peripherals enabled: LED, Timer

**Steady-state current (post init)**
- Average current: 10.46 µA  
- Average peak current (blink event): 13.60 mA

**Representative steady-state capture**
<figure>
  <img src="assets/power/ppk_blinky_steady_1.png" width="1200" alt="Blinky running with 500 ms period and 50% duty cycle">
  <figcaption><em>Blinky running with 500 ms period and 50% duty cycle</em></figcaption>
</figure>
</p>

---

## Informational: Boot Transient

Boot current is recorded separately as a transient event.

- Average boot peak current: 53.20 mA 
- Observations: Generally stable across boots, logging enabled

**Representative boot capture**
<figure>
  <img src="assets/power/ppk_blinky_boot_1.png" width="1200" alt="Boot + init power sequence">
  <figcaption><em>Boot + init power sequence</em></figcaption>
</figure>
</p>
