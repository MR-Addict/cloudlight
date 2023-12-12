// get LED status
String getLED() {
  bool status = digitalRead(LED);
  String message = String("{\"led\":\"") + (status ? "on" : "off") + "\"}";
  return message;
}

// toggle LED status
String toggleLED(boolean status) {
  digitalWrite(LED, status);
  String message = String("{\"led\":\"") + (status ? "on" : "off") + "\"}";
  return message;
}
