void connectionLostCallback(WiFiEvent_t event, WiFiEventInfo_t info) {
  if (isAPMode)return;
  WiFi.reconnect();
  Serial.println("WiFi connection lost. Try to reconnect...");
}

void setupAP(const char* hostname) {
  isAPMode = true;
  digitalWrite(builtInLED, HIGH);

  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.hostname(hostname);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(hostname);
  Serial.println("Enter AP mode");
}

bool setupStation(const char* ssid, const char* password, const char* hostname) {
  isAPMode = false;
  digitalWrite(builtInLED, LOW);

  WiFi.hostname(hostname);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint8_t attempts = 0, maxAttempts = 30;
  Serial.printf("Connecting to %s", ssid);
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(500);
    Serial.print('.');
    attempts++;
  }
  Serial.println();

  if (attempts < maxAttempts) {
    Serial.printf("Local IP: %s\n", WiFi.localIP().toString().c_str());
    WiFi.onEvent(connectionLostCallback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    return true;
  } else return false;
}

void init(uint32_t baud_rate, const char* hostname) {
  // init led
  Serial.begin(baud_rate);
  pinMode(builtInLED, OUTPUT);
  pinMode(buttonUser, INPUT_PULLUP);
  pinMode(buttonRest, INPUT_PULLUP);
  FastLED.addLeds<WS2812, LEDPin, GRB>(leds, LEDCount);
  FastLED.setBrightness(LEDBrightness);

  // init SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // get stored preferences
  preferences.begin("config", false);
  isDisplay = preferences.getBool("state", true);
  LEDBrightness = preferences.getUChar("brightness", 100);
  const String ssid = preferences.getString("ssid", "");
  const String password = preferences.getString("password", "");

  // init wifi
  if (ssid == "" || password == "" ) {
    isEditSetup = true;
    setupAP(hostname);
  } else if (!setupStation(ssid.c_str(), password.c_str(), hostname)) {
    isEditSetup = true;
    setupAP(hostname);
  }
}
