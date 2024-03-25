// get LED status
String getLED() {
  DynamicJsonDocument json(200);
  json["state"] = String(LEDState ? "on" : "off");
  json["count"] = LEDCount;
  json["brightness"] = LEDBrightness;

  String message;
  serializeJson(json, message);
  return message;
}

void setLED(bool state, uint8_t count, uint8_t brightness) {
  LEDState = state;
  LEDCount = count;
  LEDBrightness = brightness;
  preferences.putBool("state", state);
  preferences.putUChar("count", count);
  preferences.putUChar("brightness", brightness);
}

// refresh LED
void refreshLED() {
  static uint8_t LEDIndex = 0;
  static uint8_t ColorIndex = 0;

  EVERY_N_MILLISECONDS(50) {
    if (!LEDState)FastLED.clear();
    else {
      LEDIndex = (LEDIndex + 1) % LEDCount;
      leds[LEDIndex] = CHSV(ColorIndex++, 255, 255);

      FastLED.setBrightness(LEDBrightness);

      fadeToBlackBy(leds, LEDCount, 10);
    }
    FastLED.show();
  }
}
