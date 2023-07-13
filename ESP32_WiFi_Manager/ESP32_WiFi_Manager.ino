#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

AsyncWebServer server(80);

// Search for parameters in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";

// Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Read file from SPIFFS
String readFile(fs::FS &fs, const char* path) {
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return String();
  }
  String fileContent = file.readStringUntil('\n');
  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char* path, const char* message) {
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  initSPIFFS();

  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  ip = readFile(SPIFFS, ipPath);
  gateway = readFile(SPIFFS, gatewayPath);
  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(ip);
  Serial.println(gateway);

  if (ssid != "" && ip != "") {
    WiFi.mode(WIFI_STA);
    IPAddress localIP;
    IPAddress localGateway;
    localIP.fromString(ip);
    localGateway.fromString(gateway);
    IPAddress subnet(255, 255, 0, 0);

    if (!WiFi.config(localIP, localGateway, subnet)) {
      Serial.println("Failed to configure WiFi");
      return;
    }

    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("Connecting to WiFi...");

    unsigned long previousMillis = millis();
    const long interval = 10000;  // Interval to wait for Wi-Fi connection (milliseconds)

    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - previousMillis >= interval) {
        Serial.println("Failed to connect to WiFi");
        return;
      }
      delay(500);
    }

    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/remove", HTTP_GET, [](AsyncWebServerRequest* request) {
      if (SPIFFS.remove(ssidPath) && SPIFFS.remove(passPath)) {
        String message = "<p>Values removed</p>";
        message.replace("%VALUE%", "Values removed");
        request->send(200, "text/html", message);
        } else {
          String message = "<p>Failed to remove values</p>";
          message.replace("%VALUE%", "Failed to remove values");
          request->send(500, "text/html", message);
          }
          });

    server.begin();
  } else {
    // Access Point mode
    Serial.println("Setting up Access Point");

    WiFi.softAP("LOREIGN-WIFI-MANAGER");
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest* request) {
      int params = request->params();
      for (int i = 0; i < params; i++) {
        AsyncWebParameter* p = request->getParam(i);
        if (p->isPost()) {
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value();
            writeFile(SPIFFS, ssidPath, ssid.c_str());
            Serial.print("SSID set to: ");
            Serial.println(ssid);
          }
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value();
            writeFile(SPIFFS, passPath, pass.c_str());
            Serial.print("Password set to: ");
            Serial.println(pass);
          }
          if (p->name() == PARAM_INPUT_3) {
            ip = p->value();
            writeFile(SPIFFS, ipPath, ip.c_str());
            Serial.print("IP Address set to: ");
            Serial.println(ip);
          }
          if (p->name() == PARAM_INPUT_4) {
            gateway = p->value();
            writeFile(SPIFFS, gatewayPath, gateway.c_str());
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
          }
        }
      }
      request->send(SPIFFS, "/index.html", "text/html");
      delay(3000);
      ESP.restart();
    });

    server.begin();
  }
}

void loop() {
  // Empty loop
}
