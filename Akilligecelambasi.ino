#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "html_page.h"

const int redPin = 25;
const int greenPin = 26;
const int bluePin = 27;
const int ldrPin = 2;

const char* ssid = "Akilli_Isik";
const char* password = "12345678";

WebServer server(80);

int brightness = 50;
bool autoMode = false;
String currentMode = "kapali";
bool partyMode = false;
unsigned long lastPartyUpdate = 0;
int partyStep = 0;

const int pwmFreq = 5000;
const int pwmResolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(redPin, pwmFreq, pwmResolution);
  ledcAttach(greenPin, pwmFreq, pwmResolution);
  ledcAttach(bluePin, pwmFreq, pwmResolution);
  pinMode(ldrPin, INPUT);
  setColor(0, 0, 0);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP baslatildi");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/control", HTTP_POST, handleControl);

  server.begin();
}

void loop() {
  server.handleClient();

  if (partyMode && currentMode == "party") {
    if (millis() - lastPartyUpdate > 500) {
      partyAnimation();
      lastPartyUpdate = millis();
    }
  }

  if (autoMode) {
    checkAutoMode();
  }
  delay(10);
}

void setColor(int red, int green, int blue) {
  int r = map(red * brightness, 0, 25500, 0, 255);
  int g = map(green * brightness, 0, 25500, 0, 255);
  int b = map(blue * brightness, 0, 25500, 0, 255);
  ledcWrite(redPin, r);
  ledcWrite(greenPin, g);
  ledcWrite(bluePin, b);
}

void partyAnimation() {
  switch (partyStep) {
    case 0: setColor(255, 0, 0); break;
    case 1: setColor(255, 127, 0); break;
    case 2: setColor(255, 255, 0); break;
    case 3: setColor(0, 255, 0); break;
    case 4: setColor(0, 0, 255); break;
    case 5: setColor(75, 0, 130); break;
    case 6: setColor(148, 0, 211); break;
  }
  partyStep = (partyStep + 1) % 7;
}

void checkAutoMode() {
  bool isDark = digitalRead(ldrPin) == LOW;
  if (isDark && currentMode != "beyaz") {
    currentMode = "beyaz";
    setColor(255, 255, 255);
  } else if (!isDark && currentMode == "beyaz") {
    currentMode = "kapali";
    setColor(0, 0, 0);
  }
}

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleStatus() {
  DynamicJsonDocument doc(1024);
  doc["mode"] = currentMode;
  doc["brightness"] = brightness;
  doc["autoMode"] = autoMode;
  doc["isDark"] = digitalRead(ldrPin) == LOW;
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleControl() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));
    String action = doc["action"];

    if (action == "setMode") {
      String mode = doc["mode"];
      currentMode = mode;
      partyMode = false;

      if (mode == "kapali") setColor(0, 0, 0);
      else if (mode == "beyaz") setColor(255, 255, 255);
      else if (mode == "mavi") setColor(0, 0, 255);
      else if (mode == "kirmizi") setColor(255, 0, 0);
      else if (mode == "yesil") setColor(0, 255, 0);
      else if (mode == "party") {
        partyMode = true;
        partyStep = 0;
      }

    } else if (action == "toggleAuto") {
      autoMode = !autoMode;

    } else if (action == "setBrightness") {
      brightness = doc["brightness"];
      if (currentMode == "beyaz") setColor(255, 255, 255);
      else if (currentMode == "mavi") setColor(0, 0, 255);
      else if (currentMode == "kirmizi") setColor(255, 0, 0);
      else if (currentMode == "yesil") setColor(0, 255, 0);
    }
  }
  server.send(200, "text/plain", "OK");
}
