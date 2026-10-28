# Wiring & Block Diagram

```mermaid
flowchart LR
    PIR[PIR motion sensor] -->|GPIO 27| ESP[ESP32 controller]
    DHT[DHT22 temperature / humidity] -->|GPIO 4| ESP
    ESP -->|GPIO 26| R1[Relay channel 1]
    ESP -->|GPIO 25| R2[Relay channel 2]
    R1 --> L[Low-voltage light / LED]
    R2 --> F[Low-voltage DC fan]
    ESP <-->|Wi-Fi HTTP| PHONE[Phone / laptop browser]
```

## Important wiring notes
- DHT22 DATA → GPIO 4, VCC → 3.3 V (check your module), GND → GND.
- PIR OUT → GPIO 27; ensure its output signal is 3.3 V-safe.
- Relay IN1 → GPIO 26; Relay IN2 → GPIO 25; use a compatible relay board.
- Use a suitable separate supply for the DC fan/light where needed, and follow the relay board's isolation and power requirements.
- Share ground for low-voltage control circuits where required by the module design.
- Start with LEDs and a small low-voltage DC motor/fan. Never put AC mains on a breadboard.
