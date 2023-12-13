// get LED status
String getLED() {
  DynamicJsonDocument json(200);
  json["state"] = String(isDisplay ? "on" : "off");
  json["brightness"] = LEDBrightness;

  String message;
  serializeJson(json, message);
  return message;
}

// refresh LED
void refreshLED() {
  uint8_t sinBeatA = beatsin8(5, 0, 255, 0, 0);
  uint8_t sinBeatB = beatsin8(10, 0, 255, 0, 0);

  EVERY_N_MILLISECONDS(10) {
    FastLED.setBrightness(LEDBrightness);
    if (isDisplay) fill_rainbow(leds, LEDCount, (sinBeatA + sinBeatB) / 2, 8);
    else FastLED.clear();
    FastLED.show();
  }
}
