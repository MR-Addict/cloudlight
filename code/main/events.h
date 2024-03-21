void listenReboot() {
  if (isReboot) {
    Serial.println("Rebooting...");
    delay(100);
    ESP.restart();
  }
}

void listenButtonPressed() {
  if (!digitalRead(buttonRest) && !isAPMode) {
    delay(10);
    if (!digitalRead(buttonRest)) {
      uint64_t last = millis();
      while (!digitalRead(buttonRest) && millis() - last <= 3000);
      if (millis() - last > 3000) setupAP(hostname);
    }
  }
}
