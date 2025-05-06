#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>

int relayState = LOW;
Adafruit_BMP085 bmp;
int MQ_pin = 12;
const char* ssid = "Moto";
const char* password = "agrim310108";
String toLowerCase(String str) {
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  return str;
}

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 2000; // Update every 2 seconds

int temp = 108;
int pressure = 108;
int altitude = 108;
int co_level = 108;

void handleSensorData() {
  temp = bmp.readTemperature();
  Serial.println(temp);
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude(101500);
  co_level = analogRead(MQ_pin);
  // if(temp<40){
  //   Serial.println("stop");
  // }
  // if(pressure<6000){
  //   Serial.println("stop");
  // }
  // if(co_level>2235){
  //   Serial.println("stop");
  // }
  // if(relayState==1){
  //     Serial.println("stop");
  // }
  delay(3000);
}
ESP8266WebServer server(80);
void handleRoot() {
  // Send sensor data as JSON
  Serial.println(String(temp));
 String json = "{\"temperature\": " + String(temp) + 
                ", \"pressure\": " + String(pressure) + 
                ", \"altitude\": " + String(altitude) + 
                ", \"co_level\": " + String(co_level) + 
                ", \"prblm\": " + String(relayState) + 
                "}";

  server.sendHeader("Access-Control-Allow-Origin", "*"); // CORS support
  server.send(200, "application/json", json);
}
void setup() {
  Serial.begin(9600);     // Monitor for debugging at 9600
if(!bmp.begin()){
  Serial.println("couldnotfind a valid bmp sensor");
while(1){}
}
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, handleRoot); // Serve sensor data

  server.on("/command", []() {
    String action = server.arg("action");
    action = toLowerCase(action);
    Serial.println( action);

  
    });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Rotate e and eb in opposite directions
   server.handleClient();
  handleSensorData();
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
  }
  
}