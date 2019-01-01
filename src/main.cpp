#include <WiFi.h>
#include <ArduinoOTA.h>

// needed for WifiManager
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include "test.h"
#include "bluetooth.h"

#define LED_BUILTIN 2

AsyncWebServer server(80);
DNSServer dns;
// SimpleBLE SerialBT;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  // test
  pinMode(LED_BUILTIN, OUTPUT);

  // if(!SerialBT.begin("QlockToo")){
  //   Serial.println("An error occurred initializing Bluetooth");
  // }

  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("QlockToo");

  setupBT();

  ArduinoOTA.setHostname("QlockToo");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();

  loopBT();

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(150);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
  // printIets("Ik doe ook andere dingen");
}