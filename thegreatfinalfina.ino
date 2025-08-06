#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
ESP8266WebServer server(80);

const char* ssid = "D_D";
const char* password = "don't know";
int MQ_pin = 12;

unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 3000; // 3 seconds

int temp = 108;
int pressure = 108;
int altitude = 108;
int co_level = 108;
int relayState = LOW;

// ----------- Utilities -----------

String toLowerCase(String str) {
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  return str;
}

// ----------- Sensor Reading (Non-blocking) -----------

void updateSensorData() {
  temp = bmp.readTemperature();
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude(101500);
  co_level = analogRead(MQ_pin);
}

// ----------- Routes -----------

void handleRoot() {
  String json = "{\"temperature\": " + String(temp) + 
                ", \"pressure\": " + String(pressure) + 
                ", \"altitude\": " + String(altitude) + 
                ", \"co_level\": " + String(co_level) + 
                ", \"prblm\": " + String(relayState) + 
                "}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find BMP sensor");
    while (1) {}
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP:");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);

  server.on("/command", []() {
    String action = server.arg("action");
    action = toLowerCase(action);
    Serial.println(action);

    // handle action if needed

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Action: " + action);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // ✅ Always keep this first for responsive API

  unsigned long now = millis();
  if (now - lastSensorTime >= sensorInterval) {
    lastSensorTime = now;
    updateSensorData();   // ✅ non-blocking sensor read
  }

  // You can do more non-blocking work here
}
