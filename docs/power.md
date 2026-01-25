# Power Report
This document contains power profiling results for **embr**, captured with the **Nordic Power Profiler Kit II (PPK2)**.
Values are intended to provide a repeatable baseline and enable apples-to-apples comparison as features are added
(e.g., DMIC bring-up, wake-word inference bursts, Thread/Matter activity).

---

## Measurement Setup
<figure>
  <img src="assets/power/t53_ppk2.png" width="600" alt="Thingy:53 connected to the PPK2)">
  <figcaption><em>Thingy:53 connected to the PPK2 via the current measurement and debug board.</em></figcaption>
</figure>
</p>

**Tool**: Nordic Power Profiler Kit II (PPK2)  
**Measurement mode**: Ammeter mode (target powered through PPK2)  
**Supply voltage**: 3.12 V (measured at Thingy:53 VIN with DMM; PPK2 VOUT → VIN)  
**Target hardware**: Nordic Thingy:53  
**Firmware state**: DMIC continuous capture (tag v0.2.0)  
**Logging**: RTT

**Notes**
- Capture windows are taken after initialization settles unless stated otherwise.
- Steady-state windows are long enough to include multiple full behavior cycles.
- Boot transient is treated separately because it can distort short-window averages.

---

## Interpretation
The measurements below currently establish:
- A general baseline for:
  - Verifying the system reaches low-power idle between specific events (e.g LED blink)
  - Comparing incremental power cost as features are added
  - Identifying unexpected background activity early

- DMIC audio capture baseline:
  - 16 kHz sample rate
  - Continuous capture, no sleep/idle

Future profiles planned:
- Wake-word bursts, with and without subsequent inference
- Thread join + idle
- Radio TX/RX bursts
- End-to-end voice-triggered lighting control path

---

## Results Summary
**Notes**
- Results are presented in reverse chronological order. Measurements for the latest firmware/feature set will appear first.
- For all results:
  - Trial count: 3
  - Capture window: 30 s per trial (steady-state time post init will be shorter)
- Measurements:
  -  Average current and average peak current
  -  Measurements from DMIC bring-up and later also include: min/max, range, standard deviation (std dev), and coefficient of variance (CV)

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
