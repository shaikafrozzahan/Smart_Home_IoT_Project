/*
  Smart Home IoT Automation - ESP32
  Sensors: DHT22 (GPIO 4), PIR (GPIO 27)
  Outputs: light relay (GPIO 26), fan relay (GPIO 25)

  Safety: prototype with low-voltage demo loads only.
  Do not connect household AC mains unless the complete installation is
  designed and installed by a qualified electrician.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define DHT_PIN 4
#define DHT_TYPE DHT22
#define PIR_PIN 27
#define LIGHT_RELAY_PIN 26
#define FAN_RELAY_PIN 25

// Set true for common active-low relay modules; set false for active-high.
const bool RELAY_ACTIVE_LOW = true;

DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);

bool lightOn = false;
bool fanOn = false;
bool autoLight = true;
bool motionDetected = false;
unsigned long lastMotionMillis = 0;
const unsigned long MOTION_HOLD_MS = 15000;

const char PAGE[] PROGMEM = R"HTML(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Smart Home IoT</title>
<style>
:root{font-family:system-ui,Arial,sans-serif;color:#172033;background:#eef3f8}
body{margin:0}.hero{padding:28px 18px;background:linear-gradient(120deg,#172554,#2563eb);color:white}
main{max-width:900px;margin:18px auto;padding:0 14px}
h1{margin:0 0 6px;font-size:28px}.muted{opacity:.8}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));gap:14px}
.card{background:white;border-radius:16px;padding:18px;box-shadow:0 4px 18px #14213d12}
.metric{font-size:30px;font-weight:750;margin-top:8px}
button{border:0;border-radius:10px;padding:11px 14px;margin:5px 5px 0 0;background:#2563eb;color:white;font-weight:650;cursor:pointer}
button.secondary{background:#475569}button.active{background:#15803d}
.pill{display:inline-block;border-radius:99px;padding:5px 10px;background:#e2e8f0}
footer{font-size:13px;color:#64748b;padding:20px 2px}
</style>
</head>
<body>
<header class="hero"><h1>🏠 Smart Home IoT</h1><div class="muted">ESP32 live room monitor & automation</div></header>
<main>
<div class="grid">
<section class="card"><div>🌡️ Temperature</div><div class="metric" id="temp">-- °C</div><div class="muted">DHT22 sensor</div></section>
<section class="card"><div>💧 Humidity</div><div class="metric" id="hum">-- %</div><div class="muted">Relative humidity</div></section>
<section class="card"><div>🚶 Motion</div><div class="metric" id="motion">--</div><div class="muted">PIR sensor</div></section>
<section class="card"><div>💡 Light</div><div class="metric" id="light">--</div><button id="lightBtn" onclick="toggleDevice('light')">Toggle light</button></section>
<section class="card"><div>🌀 Fan</div><div class="metric" id="fan">--</div><button id="fanBtn" onclick="toggleDevice('fan')">Toggle fan</button></section>
<section class="card"><div>⚙️ Automation</div><div class="metric" id="auto">--</div><button id="autoBtn" onclick="toggleAuto()">Toggle auto-light</button></section>
</div>
<footer>Refreshes every 2 seconds. For a trusted local network only; no login is configured. Use low-voltage demonstration loads.</footer>
</main>
<script>
let state={light:false,fan:false,autoLight:true};
async function update(){
 try{
  const r=await fetch('/api'); const d=await r.json(); state=d;
  document.getElementById('temp').textContent=d.temperature===null?'-- °C':d.temperature.toFixed(1)+' °C';
  document.getElementById('hum').textContent=d.humidity===null?'-- %':d.humidity.toFixed(1)+' %';
  document.getElementById('motion').textContent=d.motion?'Detected':'Clear';
  document.getElementById('light').textContent=d.light?'ON':'OFF';
  document.getElementById('fan').textContent=d.fan?'ON':'OFF';
  document.getElementById('auto').textContent=d.autoLight?'ON':'OFF';
  document.getElementById('lightBtn').className=d.light?'active':'';
  document.getElementById('fanBtn').className=d.fan?'active':'';
  document.getElementById('autoBtn').className=d.autoLight?'active':'secondary';
 }catch(e){document.getElementById('motion').textContent='Offline';}
}
async function toggleDevice(device){
 const value=state[device]?'0':'1';
 await fetch('/control?'+device+'='+value,{method:'POST'}); update();
}
async function toggleAuto(){
 await fetch('/control?auto='+(state.autoLight?'0':'1'),{method:'POST'}); update();
}
update();setInterval(update,2000);
</script>
</body></html>
)HTML";

void writeRelay(uint8_t pin, bool on) {
  digitalWrite(pin, RELAY_ACTIVE_LOW ? (on ? LOW : HIGH) : (on ? HIGH : LOW));
}

void sendState() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  String json = "{";
  json += "\"temperature\":";
  json += isnan(t) ? "null" : String(t, 1);
  json += ",\"humidity\":";
  json += isnan(h) ? "null" : String(h, 1);
  json += ",\"motion\":";
  json += motionDetected ? "true" : "false";
  json += ",\"light\":";
  json += lightOn ? "true" : "false";
  json += ",\"fan\":";
  json += fanOn ? "true" : "false";
  json += ",\"autoLight\":";
  json += autoLight ? "true" : "false";
  json += "}";
  server.send(200, "application/json", json);
}

void handleControl() {
  if (server.hasArg("light")) {
    lightOn = server.arg("light") == "1";
    // Manual control takes priority until auto mode is toggled/re-enabled.
    writeRelay(LIGHT_RELAY_PIN, lightOn);
  }
  if (server.hasArg("fan")) {
    fanOn = server.arg("fan") == "1";
    writeRelay(FAN_RELAY_PIN, fanOn);
  }
  if (server.hasArg("auto")) {
    autoLight = server.arg("auto") == "1";
  }
  server.send(200, "application/json", "{\"ok\":true}");
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);
  writeRelay(LIGHT_RELAY_PIN, false);
  writeRelay(FAN_RELAY_PIN, false);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Dashboard: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi connection failed. Check SSID/password and restart.");
  }

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", PAGE);
  });
  server.on("/api", HTTP_GET, sendState);
  server.on("/control", HTTP_POST, handleControl);
  server.begin();
}

void loop() {
  server.handleClient();
  motionDetected = digitalRead(PIR_PIN) == HIGH;
  if (motionDetected) {
    lastMotionMillis = millis();
  }

  if (autoLight) {
    bool shouldLight = motionDetected || (millis() - lastMotionMillis < MOTION_HOLD_MS);
    if (shouldLight != lightOn) {
      lightOn = shouldLight;
      writeRelay(LIGHT_RELAY_PIN, lightOn);
    }
  }
}
