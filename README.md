# IIT Bhubaneswar — Live Bus Tracking System

**Live tracking dashboard for the 6 campus buses at IIT Bhubaneswar.**  
Retrofit each bus with a low-cost ESP32 + cellular + GPS tracker that pushes coordinates to the Blynk IoT cloud. A public web dashboard (Blynk Web Dashboard) displays each bus on a map with ~5–10s refresh.

---

## Table of contents
1. [Overview](#overview)  
2. [Features](#features)  
3. [System architecture](#system-architecture)  
4. [Hardware requirements](#hardware-requirements)  
5. [Software & libraries](#software--libraries)  
6. [Blynk cloud setup (step-by-step)](#blynk-cloud-setup-step-by-step)  
7. [Hardware assembly & wiring](#hardware-assembly--wiring)  
8. [Deployment & testing checklist](#deployment--testing-checklist)  
9. [Troubleshooting & tips](#troubleshooting--tips)  
10. [Safety](#safety)  
11. [Future improvements](#future-improvements)  
12. [License & credits](#license--credits)

---

## Overview
Large campuses suffer from schedule uncertainty. This project delivers a simple, reliable, web-based map showing exact, live locations of all campus buses so students and staff can time their arrival. The design prioritizes low cost and reliability by leveraging Blynk IoT cloud for backend & web dashboard, while hardware units focus on robust data capture and transmission.

---

## Features
- Real-time tracking (~5–10 s refresh)
- Web dashboard (no app install)
- Multi-vehicle support (6 buses, unique icons)
- Low-cost hardware and easily reproducible units
- Cellular connectivity (GPRS/4G) — independent of campus Wi-Fi
- Robust power: powered from the bus electrical system (12V/24V → 5V)

---

## System architecture
```
[GPS module]  [ESP32 (TTGO T-Call)]  [Cellular modem]  --cellular-->  Blynk IoT Cloud  --> Web Dashboard (Map)
      |               |                                  |
      |---------------|----------------------------------|
                 On-bus power (via LM2596 buck converter)
```

Each tracker:
- Reads GPS NMEA from the GPS module
- Sends parsed coordinates and device metadata to Blynk datastreams
- Blynk Web Dashboard map widget shows all devices in real time

---

## Hardware requirements (per bus)
| Component | Example model | Qty | Purpose |
|---|---:|:---:|---|
| Microcontroller + Cellular | TTGO T-Call (ESP32 + SIM800L) | 1 | Tracker brain & modem |
| GPS module | NEO-6M / NEO-7M | 1 | Satellite fix |
| DC-DC Buck converter | LM2596 | 1 | 12/24V → 5V stable supply |
| SIM card | Any provider (Jio/Airtel/etc.) | 1 | Cellular data |
| Cellular antenna | SMA / u.FL antenna | 1 | Improve cellular reception |
| GPS antenna | Active ceramic patch antenna | 1 | Improve GPS reception |
| Enclosure | IP65, non-metallic | 1 | Protection from dust/vibration |
| Wires / soldering kit | — | — | Reliable, vibration-proof connections |

---

## Software & libraries
- Arduino IDE or PlatformIO
- Blynk Cloud account
- Core libraries commonly used for this build:
  - Blynk library supporting ESP32 + GSM
  - TinyGPS++ (NMEA parsing / GPS)
  - Any serial/UART helper libraries needed by your board

---

## Blynk cloud setup (step-by-step)
1. Create a Blynk account and log in.  
2. Create a new **Template** (name it appropriately for the project). Select `ESP32` as the hardware type.  
3. Add Datastreams to the template:
   - `GPS Coordinates` → Virtual Pin `V0` → Data type: **Location**
   - `Bus ID` → Virtual Pin `V1` → Data type: **String**
4. Create one device entry per bus (six devices). Each device will be registered under the template.  
5. Configure the **Web Dashboard** for the template:
   - Add a **Map** widget
   - Point the widget to the GPS Coordinates datastream
   - Enable options to show all devices and device names on hover
6. Publish or share the dashboard URL to allow web access (internal or public depending on your sharing settings).

> Note: Blynk issues per-device credentials and device entries during the device creation process. The cloud handles storage, mapping, and visualization once devices publish location data to the configured datastreams.

---

## Hardware assembly & wiring
> Workbench testing first — do not wire to a vehicle until bench tests pass.

### Buck converter setup (LM2596)
1. Connect a stable 12V source to the converter input.  
2. Measure the converter output with a multimeter and adjust the potentiometer until the output reads **5.00 V**. Confirm the reading under a light load.

### Recommended connections
- **Power**: Buck converter 5V → ESP32 5V pin; buck converter GND → ESP32 GND.  
- **GPS**: GPS module VCC → module-appropriate voltage (check module specs), GPS GND → GND, GPS TX → an available UART RX pin on the ESP32, GPS RX → the corresponding UART TX pin on the ESP32.  
- **Cellular antenna**: Connect to the modem's antenna connector and place clear of heavy metal or shielding.  
- Solder and mechanically secure all critical connections. Use strain relief for cables and mount the electronics securely inside an IP65 enclosure. Route antennas to locations that provide the best signal (roof or window areas are ideal).

---

## Deployment & testing checklist
1. Bench test power, GPS reception, and cellular registration.  
2. Verify each tracker device appears in the Blynk Web Dashboard device list and shows location updates on the dashboard map.  
3. Confirm map markers and hover labels display correctly.  
4. Mount the unit in the enclosure, secure antennas, and reinstall on the vehicle.  
5. Field test by driving the vehicle along routes, verify continuous updates and acceptable latency.  
6. Monitor data usage and telemetry behavior for the chosen SIM plan.

---

## Troubleshooting & tips ( if implementing ) 
- **No GPS fix:** Ensure antenna has a clear view of the sky, check wiring, and allow several minutes for a first fix.  
- **SIM not registering / no data:** Verify the SIM has an active data plan and correct APN settings; test the SIM in a phone to confirm network coverage.  
- **Frequent disconnects:** Inspect antenna routing, check for loose connectors, and consider a higher-gain antenna if coverage is marginal.  
- **Power instability:** Ensure the buck converter is adjusted correctly and remains at 5.00 V under load; add bulk capacitance on the 5V rail if spikes occur.  
- **Reduce data usage:** Increase location-send interval, send updates only when the vehicle moves, or implement delta reporting.

---

## Safety
- Performed initial wiring and testing on a bench power supply, not the vehicle battery.  
- Confirmed buck converter output with a multimeter before connecting the tracker.  

---

## Future improvements
- ETA estimation based on historical route data.  
- In-vehicle logging with buffering to survive temporary connectivity loss.  
- Remote OTA updates and health/status telemetry (battery, signal strength).  
- Integration with campus notification channels for delay alerts.

---

## License & credits
- Built with the Blynk platform, TinyGPS++, and the Arduino ecosystem.

---

