/*
  LUMI – example ESP32 REST endpoint  (UNTESTED EXAMPLE – not verified on hardware)

  Serves JSON at  http://<esp32-ip>/api/sensors  for the LUMI web app.
  The read*() functions below are STUBS that return NAN, so every field is sent as
  null until you replace them with real sensor code (e.g. MAX30102, DHT22, MQ-135).
  Never fill them with made-up numbers: LUMI will show whatever you send as LIVE data.

  Board: ESP32 (Arduino core). Libraries: only WiFi + WebServer (built in).
*/
#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// ---- TODO: replace stubs with real sensor reads (return NAN if no valid reading) ----
float readHeartRate()  { return NAN; }  // BPM
float readSpO2()       { return NAN; }  // %
float readTemperature(){ return NAN; }  // °C
float readHumidity()   { return NAN; }  // %
float readGasRaw()     { return NAN; }  // raw sensor value
// ------------------------------------------------------------------------------------

const char* gasStatus(float raw) {
  if (isnan(raw)) return nullptr;
  // Example thresholds only. Calibrate for your sensor.
  if (raw < 200) return "good";
  if (raw < 400) return "moderate";
  return "poor";
}

void addNum(String &s, const char* key, float v, int dec, bool last = false) {
  s += "\""; s += key; s += "\":";
  if (isnan(v)) s += "null"; else s += String(v, dec);
  if (!last) s += ",";
}

void cors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.sendHeader("Access-Control-Allow-Private-Network", "true");
  server.sendHeader("Cache-Control", "no-store");
}

void handleSensors() {
  float gas = readGasRaw();
  const char* gs = gasStatus(gas);
  String j = "{\"device\":\"ESP32-LUMI\",";
  addNum(j, "heart_rate", readHeartRate(), 0);
  addNum(j, "spo2", readSpO2(), 0);
  addNum(j, "temperature", readTemperature(), 1);
  addNum(j, "humidity", readHumidity(), 0);
  j += "\"gas_status\":"; j += gs ? String("\"") + gs + "\"" : String("null"); j += ",";
  addNum(j, "gas_value", gas, 0, true);
  j += "}";
  cors();
  server.send(200, "application/json", j);
}

void handleOptions() { cors(); server.send(204); }

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) { delay(400); Serial.print("."); }
  Serial.print("\nConnected. Enter this in LUMI: http://");
  Serial.println(WiFi.localIP());

  server.on("/api/sensors", HTTP_GET, handleSensors);
  server.on("/api/sensors", HTTP_OPTIONS, handleOptions);
  server.begin();
}

void loop() { server.handleClient(); }
