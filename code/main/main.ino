#include <WiFi.h>
#include <SPIFFS.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>

bool isReboot = false;
bool isAPMode = false;
bool isEditSetup = false;

bool isDisplay = true;
const uint8_t LEDPin = 4;
const uint8_t LEDCount = 29;
uint8_t LEDBrightness = 100;

const uint8_t buttonRest = 0;
const uint8_t buttonUser = 5;
const uint8_t builtInLED = 2;
const char* hostname = "cloudlight";

CRGB leds[LEDCount];
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
  refreshLED();
  listenReboot();
  listenButtonPressed();
}
