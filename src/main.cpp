#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "web_interface.h"

// Pin tanimlamalari
const int redPin = 25;
const int greenPin = 26;
const int bluePin = 27;
const int ldrPin = 2;

// WiFi ayarlari
const char* ssid = "Akilli_Isik";
const char* password = "12345678";

WebServer server(80);

// Degiskenler
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
  Serial.println("Akilli Isik Sistemi Baslatiliyor...");
  
  // PWM kanallarini ayarla
  ledcAttach(redPin, pwmFreq, pwmResolution);
  ledcAttach(greenPin, pwmFreq, pwmResolution);
  ledcAttach(bluePin, pwmFreq, pwmResolution);
  
  // LDR pin ayari
  pinMode(ldrPin, INPUT);
  
  // Baslangicta LED'i kapat
  setColor(0, 0, 0);
  
  // WiFi Access Point baslat
  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP baslatildi");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP adresi: ");
  Serial.println(WiFi.softAPIP());
  
  // Web sunucu rotalarini ayarla
  setupWebServer();
  
  server.begin();
  Serial.println("Web sunucu baslatildi");
  Serial.println("Sistem hazir!");
}

void loop() {
  server.handleClient();
  
  // Party modu animasyonu
  if (partyMode && currentMode == "party") {
    if (millis() - lastPartyUpdate > 500) {
      partyAnimation();
      lastPartyUpdate = millis();
    }
  }
  
  // Otomatik mod kontrolu
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
  switch(partyStep) {
    case 0: setColor(255, 0, 0); break;     // Kirmizi
    case 1: setColor(255, 127, 0); break;   // Turuncu
    case 2: setColor(255, 255, 0); break;   // Sari
    case 3: setColor(0, 255, 0); break;     // Yesil
    case 4: setColor(0, 0, 255); break;     // Mavi
    case 5: setColor(75, 0, 130); break;    // Indigo
    case 6: setColor(148, 0, 211); break;   // Mor
  }
  partyStep = (partyStep + 1) % 7;
}

void checkAutoMode() {
  bool isDark = digitalRead(ldrPin) == HIGH;
  
  if (isDark && currentMode != "beyaz") {
    currentMode = "beyaz";
    setColor(255, 255, 255);
    Serial.println("Auto mod: Karanlik algilandi, isik acildi");
  } else if (!isDark && currentMode == "beyaz") {
    currentMode = "kapali";
    setColor(0, 0, 0);
    Serial.println("Auto mod: Aydinlik algilandi, isik kapatildi");
  }
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/control", HTTP_POST, handleControl);
  server.onNotFound(handleNotFound);
}

void handleStatus() {
  DynamicJsonDocument doc(1024);
  
  doc["mode"] = currentMode;
  doc["brightness"] = brightness;
  doc["autoMode"] = autoMode;
  doc["isDark"] = digitalRead(ldrPin) == HIGH;
  doc["partyMode"] = partyMode;
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void handleControl() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    
    if (error) {
      server.send(400, "text/plain", "JSON Parse Error");
      return;
    }
    
    String action = doc["action"];
    
    if (action == "setMode") {
      String mode = doc["mode"];
      setLightMode(mode);
    } else if (action == "toggleAuto") {
      autoMode = !autoMode;
      Serial.print("Auto mod: ");
      Serial.println(autoMode ? "Aktif" : "Pasif");
    } else if (action == "setBrightness") {
      brightness = doc["brightness"];
      brightness = constrain(brightness, 1, 100);
      
      // Mevcut modu yeniden uygula
      setLightMode(currentMode);
      Serial.print("Parlaklik ayarlandi: ");
      Serial.println(brightness);
    }
    
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "No Data");
  }
}

void setLightMode(String mode) {
  currentMode = mode;
  partyMode = false;
  
  Serial.print("Mod degistirildi: ");
  Serial.println(mode);
  
  if (mode == "kapali") {
    setColor(0, 0, 0);
  } else if (mode == "beyaz") {
    setColor(255, 255, 255);
  } else if (mode == "mavi") {
    setColor(0, 0, 255);
  } else if (mode == "kirmizi") {
    setColor(255, 0, 0);
  } else if (mode == "yesil") {
    setColor(0, 255, 0);
  } else if (mode == "party") {
    partyMode = true;
    partyStep = 0;
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Sayfa bulunamadi");
}
