# Smart Home IoT Automation System (ESP32)

A beginner-friendly IoT project for monitoring room temperature and motion, and controlling a light and fan from a phone/laptop browser on the same Wi-Fi network.

## Features
- ESP32 hosts a local web dashboard; no cloud account required.
- DHT22 temperature/humidity readings.
- PIR motion sensor.
- Two relay outputs for a low-voltage light and fan (or LEDs/motors during prototyping).
- Manual controls and simple motion-based automatic light mode.
- Status API (`/api`) returns JSON for future app or cloud integration.

## Hardware
- ESP32 DevKit
- DHT22 temperature/humidity sensor
- HC-SR501 PIR motion sensor
- 2-channel relay module compatible with the ESP32's logic level
- Low-voltage LED lamp and small DC fan for safe demonstration
- Breadboard, jumper wires, suitable power supply

### Wiring (default pin numbers in `firmware/SmartHomeIoT.ino`)
| Component | ESP32 pin | Notes |
|---|---:|---|
| DHT22 DATA | GPIO 4 | Use a 10 kΩ pull-up to 3.3 V if your DHT22 module doesn't include one |
| PIR OUT | GPIO 27 | Power sensor according to its datasheet; output must not exceed 3.3 V |
| Light relay IN | GPIO 26 | Relay module must be compatible with ESP32 logic |
| Fan relay IN | GPIO 25 | Relay module must be compatible with ESP32 logic |
| GND | GND | Common ground for low-voltage sensor/driver circuits |

Relay modules vary: many are active-low. Change `RELAY_ACTIVE_LOW` in the sketch if needed.

## Setup
1. Install Arduino IDE.
2. Install the **ESP32 by Espressif Systems** board package.
3. Install Arduino libraries **DHT sensor library** by Adafruit and **Adafruit Unified Sensor**.
4. Open `firmware/SmartHomeIoT.ino`.
5. Replace `YOUR_WIFI_NAME` and `YOUR_WIFI_PASSWORD` with your Wi-Fi credentials.
6. Select your ESP32 board and COM port, then upload.
7. Open Serial Monitor at 115200 baud and find the IP address printed by the ESP32.
8. Connect your phone/laptop to the same Wi-Fi and open `http://ESP32_IP_ADDRESS/`.

## Demo checklist
- Dashboard opens on a phone.
- Temperature and humidity update.
- PIR status changes when movement is detected.
- Manual light/fan buttons operate the low-voltage demo loads.
- Auto-light mode switches the light on when motion is detected and off after motion clears.

## Safety
**Use low-voltage demo loads only for a student prototype.** Do not connect household AC mains, wall wiring, or a real mains-powered fan/light directly to a breadboard or relay module. Mains wiring can cause fatal shock or fire and must only be designed/installed by a qualified electrician with proper isolation, enclosure, fusing, and local-code compliance. Keep the ESP32 and low-voltage circuits isolated from mains.

## Limitations and improvements
- Dashboard is intended for a trusted local network and has no login; don't expose it to the public internet.
- The basic motion automation uses PIR state and a short timeout, not occupancy prediction.
- Add MQTT (e.g., Mosquitto), secure authentication, energy metering, notifications, or a mobile app as future work.

## Suggested resume entry (only after you build/test it)
**Smart Home IoT Monitoring and Automation using ESP32**
- Developed a local web dashboard to monitor room temperature, humidity, and motion using ESP32, DHT22, and PIR sensors.
- Implemented browser-based control of low-voltage light/fan demo loads through relay outputs.
- Added motion-triggered automatic lighting and a JSON status endpoint for IoT integration.
- Tools: Arduino IDE, ESP32, C++, DHT22, PIR, HTTP, HTML/CSS/JavaScript.

Be honest about which features you personally assembled and tested; this repository is a starter implementation, not proof that hardware tests have already been completed.
