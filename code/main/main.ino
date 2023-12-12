#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>

bool isReboot = false;
bool isAPMode = false;
bool isDisplay = false;
bool isEditSetup = false;

//const uint8_t LED = 4;
const uint8_t builtInLED = 2;
const uint8_t buttonRest = 0;
const uint8_t buttonUser = 5;
const char* hostname = "cloudlight";

AsyncWebServer server(80);
Preferences preferences;

#include "init.h"
#include "led.h"
#include "events.h"
#include "server.h"

void setup() {
  init(115200, hostname);
  setupServer();
}

void loop() {
  listenReboot();
  listenButtonPressed();
}
