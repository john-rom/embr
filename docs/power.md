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
**Latest firmware state**: WOS capture + DSP/inference  
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

Future profiles planned:
- Thread join + idle
- Radio TX/RX bursts
- End-to-end voice-triggered lighting control path

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
