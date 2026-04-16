#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ── WiFi credentials ──────────────────────────────────────
const char* ssid     = "OnePlus Nord CE 3 Lite 5G";
const char* password = "1234567qw";       

// ── LED Pins ──────────────────────────────────────────────
#define LED1 26
#define LED2 27
#define LED3 14

HTTPClient http;


// ── LED states ────────────────────────────────────────────
bool led1State = false;
bool led2State = false;
bool led3State = false;

void setup() {
Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());   // <-- open this IP in your phone browser

}

void loop() {
  http.begin("http://10.44.101.98:3000/api/state");
  if (http.GET() == 200) {
    StaticJsonDocument<128> doc;
    deserializeJson(doc, http.getString());
    digitalWrite(LED1, doc["led1"].as<bool>());
    digitalWrite(LED2, doc["led2"].as<bool>());
    digitalWrite(LED3, doc["led3"].as<bool>());
  }
  http.end();
  delay(1000);
}