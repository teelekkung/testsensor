// src/wifimanaged.cpp
#include "globals.h"
#include "wifimanaged.h"
#include <PubSubClient.h>

WiFiManager wifiManager;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
const char *mqtt_broker = "10.42.0.1";
const int mqtt_port = 1883;
const char *topic = "ALERT";
const char *datadsent = "0";
WiFiClient espClient;
PubSubClient client(espClient);

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // print the ssid that we should connect to to configure the ESP32S3
  Serial.print("Created config portal AP ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String messagedata;
    for (int i = 0; i < length; i++) {
        messagedata += (char) payload[i];
        //Serial.print((char) payload[i]);
    }
    Serial.print(messagedata);

    //messagedata = atoi(messagedata);
    if(messagedata == "1"){
      Serial.print(" Turn LED off");
      digitalWrite(12, HIGH);
    } else {
      Serial.print("Turn LED on");
      digitalWrite(12, LOW);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void wifimanagedTask(void *parameter) {
    pinMode(12, OUTPUT) ;
    wifiManager.setConfigPortalBlocking(false);
    //wifiManager.setConfigPortalTimeout(60);
    //wifiManager.setAPCallback(configModeCallback);
    if(wifiManager.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println();
        client.setServer(mqtt_broker, mqtt_port);
        client.setCallback(callback);
        while (!client.connected()) {
            String client_id = "esp32-client-";
            client_id += String(WiFi.macAddress());
            Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
            if (client.connect(client_id.c_str())) {
                Serial.println("Public EMQX MQTT broker connected");
            } else {
                Serial.print("failed with state ");
                Serial.println(client.state());
                delay(2000);
            }
        }
        if(client.subscribe(topic)){
            Serial.print("Success full subscribe to Topic : ");
            Serial.println(topic);
        }
        for(;;){
            wifiManager.process();
            client.loop();
            if(digitalRead(12) == HIGH) {
                datadsent = "1";
            } else {
                datadsent = "0";
            }
            client.publish("Status", datadsent);
            delay(2000);
        }
    } else {
        Serial.println("GoGo Restart");
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.println();
        delay(2000);
        ESP.restart();
    }
    for(;;){
        delay(20);
    }
}
