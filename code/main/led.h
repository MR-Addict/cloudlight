// get LED status
String getLED() {
  String message = String("{\"light\":\"") + (isDisplay ? "on" : "off") + "\"}";
  return message;
}

// toggle LED status
String toggleLED(boolean state) {
  isDisplay = state;
  String message = String("{\"light\":\"") + (state ? "on" : "off") + "\"}";
  return message;
}
