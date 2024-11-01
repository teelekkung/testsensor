// src/wifimanaged.cpp
#include "globals.h"
#include "wifimanaged.h"

WiFiManager wifiManager;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // print the ssid that we should connect to to configure the ESP32S3
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void wifimanagedTask(void *parameter) {
    wifiManager.setConfigPortalBlocking(false);
    wifiManager.setConfigPortalTimeout(60);
    wifiManager.setAPCallback(configModeCallback);
    if(wifiManager.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println();
    } else {
        Serial.println("Configportal running");
    }
    for(;;){
        wifiManager.process();
        delay(20);
    }
}
