# Power Report
This document contains power profiling results for **embr**, captured with the **Nordic Power Profiler Kit II (PPK2)**.
Values are intended to provide a repeatable baseline and enable apples-to-apples comparison as features are added
(e.g., DMIC bring-up, wake-word inference bursts, Thread/Matter activity).

---

## Measurement Setup

<figure>
  <img src="docs/assets/power/t53_ppk2.png" width="600" alt="Thingy:53 connected to the PPK2)">
  <figcaption><em>Thingy:53 connected to the PPK2 via the current measurement and debug board.</em></figcaption>
</figure>
</p>

**Tool:** Nordic Power Profiler Kit II (PPK2)  
**Measurement mode:** Ammeter mode (target powered through PPK2)  
**Supply voltage**: 3.12 V (measured at Thingy:53 VIN with DMM; PPK2 VOUT → VIN)  
**Target hardware:** Nordic Thingy:53  
**Firmware state:** Blinky baseline (tag v0.1.0)  
**Logging:** RTT

**Notes**
- Capture windows are taken after initialization settles unless stated otherwise.
- Steady-state windows are long enough to include multiple full behavior cycles.
- Boot transient is treated separately because it can distort short-window averages.

---

## Results Summary

### Baseline — Blinky
**Behavior**
- Blink period: 500 ms  
- Duty cycle: 50%
- Peripherals enabled: LED, Timer

**Steady-state current (after init)**
- Trial count: 3
- Capture window: 30 s per trial
- Average current: 10.46 µA  
- Average peak current (blink event): 13.60 mA

**Representative steady-state capture**
![PPK2 blinky steady-state](docs/assets/power/ppk_blinky_steady_1.png)

---

## Informational: Boot Transient

Boot current is recorded separately as a transient event.

- Trial count: 3  
- Average boot peak current: 53.20 mA 
- Observations: Generally stable across boots, logging enabled

**Representative boot capture**
![PPK2 boot transient](docs/assets/power/ppk_blinky_boot_1.png)

---

## Interpretation
These measurements establish a baseline for:
- Verifying the system reaches low-power idle between specific events (LED blink, currently)
- Comparing incremental power cost as features are added
- Identifying unexpected background activity early

Future profiles planned:
- DMIC capture
- Wake-word inference bursts
- Thread join + idle
- Radio TX/RX bursts
- End-to-end voice-triggered lighting control path
