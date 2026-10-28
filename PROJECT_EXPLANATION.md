# Project explanation for viva / interview

## Problem statement
Lights and fans in a room may be left running unnecessarily. A low-cost IoT controller can monitor room conditions and allow local remote control.

## How it works
1. The DHT22 measures temperature and relative humidity.
2. The PIR sensor outputs a digital signal when it detects movement.
3. The ESP32 reads both sensors and controls two relay outputs.
4. The ESP32 connects to the configured Wi-Fi network and serves a dashboard.
5. A phone or laptop on the same network opens the ESP32 IP address.
6. The dashboard requests `/api` every two seconds and displays the readings and output states.
7. When auto-light is enabled, motion turns the light output on and the output stays on briefly after motion clears.

## Key terms
- **IoT:** physical devices exchanging data over a network.
- **ESP32:** microcontroller with built-in Wi-Fi.
- **PIR:** passive infrared sensor for changes associated with movement.
- **Relay:** electrically controlled switch; use an appropriate driver/module.
- **HTTP:** request/response protocol used by the browser dashboard.
- **JSON:** structured text format used by the status endpoint.

## Example viva questions
**Why ESP32 instead of a basic Arduino Uno?** ESP32 includes Wi-Fi and has sufficient GPIO for this prototype.

**Does the project require internet?** No. The ESP32 and browser only need to be on the same local Wi-Fi network.

**What is the role of the relay?** It allows a low-power control signal to switch a separate load circuit, subject to the relay module's ratings and safe isolation.

**What are the limitations?** No authentication, Wi-Fi dependence for dashboard access, PIR cannot identify a person, and DHT22 readings are not instant or laboratory-grade.

**What can be improved?** Add MQTT, login/authentication, energy metering, app notifications, schedules, secure OTA updates, and temperature-based fan control with hysteresis.

## Resume honesty
Only state features that you personally assemble and verify. If you have only studied or simulated the project, label it as a prototype/study project rather than claiming hardware testing.
