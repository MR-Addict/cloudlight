// root page
void rootPage(AsyncWebServerRequest* request) {
  if (isEditSetup)request->redirect("/setup");
  else request-> send(SPIFFS, "/index.html", "text/html");
}

// setup page
void setupPage(AsyncWebServerRequest* request) {
  request-> send(SPIFFS, "/setup.html", "text/html");
}

// handle get light status
void handleGetRequest(AsyncWebServerRequest* request) {
  String message = getLED();
  request->send(200, "application/json", message);
}

// handle update light
void handlePostRequest(AsyncWebServerRequest* request) {
  String state = "";
  int brightness = -1;
  for (uint8_t i = 0; i < request->params(); i++) {
    AsyncWebParameter* p = request->getParam(i);
    if (p->name() == "state")state = p->value();
    else if (p->name() == "brightness")brightness = String(p->value()).toInt();
  }

  bool isBadRequest = false;
  if (state != "" && state != "on" && state != "off")isBadRequest = true;
  if (brightness != -1 && (brightness < 0 || brightness > 255))isBadRequest = true;
  if (isBadRequest) {
    request->send(400, "text/plain", "Bad request");
    return;
  }

  if (brightness != -1 )LEDBrightness = brightness;
  if (state != "")isDisplay = state == "on";
  request->send(200, "application/json", getLED());
}

// get credentials
void handleGetCredentials(AsyncWebServerRequest* request) {
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  String message = "{\"ssid\":\"" + ssid + "\",\"password\":\"" + password + "\"}";
  request->send(200, "application/json", message);
}

// update credentials
void handlePostCredentials(AsyncWebServerRequest* request) {
  String ssid, password;
  for (uint8_t i = 0; i < request->params(); i++) {
    AsyncWebParameter* p = request->getParam(i);
    if (p->name() == "ssid")ssid = p->value();
    else if (p->name() == "password")password = p->value();
  }

  if (ssid == "on" || password == "") {
    request->send(400, "text/plain", "Bad request");
    return;
  }

  if (ssid != "")preferences.putString("ssid", ssid);
  if (password != "")preferences.putString("password", password);

  String ssid_stored = preferences.getString("ssid", "");
  String password_stored = preferences.getString("password", "");

  String message = "{\"ssid\":\"" + ssid_stored + "\",\"password\":\"" + password_stored + "\"}";
  request->send(200, "application/json", message);
}

void handleReboot(AsyncWebServerRequest* request) {
  isReboot = true;
  request->send(200, "text/plain", "ok");
}

// handle OPTIONS and 404 request
void handleNotFound(AsyncWebServerRequest* request) {
  String url = request->url();
  if (request->method() == HTTP_OPTIONS) request->send(200);
  else request->send(404, "text/plain", "404");
}

// setup server
void setupServer() {
  DefaultHeaders::Instance().addHeader("Server", "ESP32");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.serveStatic("/", SPIFFS, "/");

  server.on("/", HTTP_GET, rootPage);
  server.on("/setup", HTTP_GET, setupPage);

  server.on("/api/light", HTTP_GET, handleGetRequest);
  server.on("/api/light", HTTP_POST, handlePostRequest);

  server.on("/api/credentials", HTTP_GET, handleGetCredentials);
  server.on("/api/credentials", HTTP_POST, handlePostCredentials);

  server.on("/api/reboot", HTTP_POST, handleReboot);

  server.onNotFound(handleNotFound);
  server.begin();
}
