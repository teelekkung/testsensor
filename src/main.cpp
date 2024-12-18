#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <FastLED.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

#define TEMP6000    4
#define TTP223      5
#define RGB_PIN     6
#define LED_PWM     7
#define RGB_NUMS    8
#define ahtbmpsda   8
#define ahtbmpscl   9
#define i2clcdsda   10
#define i2clcdscl   11
#define i2clcdaddr  0x25
#define ledbit1     15
#define ledbit2     16
#define ledbit3     17

void callback(char *topic, byte *payload, unsigned int length) ;

const int TIMEZONE_OFFSET = 7 ;
const char *mqtt_broker = "10.42.0.1";
const int mqtt_port = 1883;
const char *topic = "ALERT";
unsigned long ploop0 = 0 ;
const long loop0 = 20 ;
unsigned long ploop1 = 0 ;
const long loop1 = 1000 ;
unsigned long ploop2 = 0 ;
const long loop2 = 2000 ;

TwoWire ahtbmp = TwoWire(0) ;
TwoWire i2clcd = TwoWire(1) ;
RTC_DS1307 rtc;
LiquidCrystal_PCF8574 lcd(i2clcdaddr) ;
Adafruit_AHTX0 aht ;
Adafruit_BMP280 bmp(&ahtbmp) ;
CRGB leds[RGB_NUMS] ;
WiFiManager wifiManager ;
// WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40) ;
WiFiClient espClient ;
PubSubClient client(espClient) ;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200) ;
    // // //
    pinMode(TEMP6000, INPUT) ;
    pinMode(TTP223, INPUT) ;    
    // // //
    if(ahtbmp.begin(ahtbmpsda, ahtbmpscl)) {
        Serial.println("Successfull create ahtbmp i2c bus") ;
    } else {
        Serial.println("Couldn't create ahtbmp bus") ;
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.println("BYPASS ") ;
        delay(1000) ;
    }
    if(i2clcd.begin(i2clcdsda, i2clcdscl)) {
        Serial.println("Successfull create i2clcd i2c bus") ;
    } else {
        Serial.println("Couldn't create i2clcd bus") ;
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.println("BYPASS ") ;
        delay(1000) ;
    }
    // // //
    if(rtc.begin(&i2clcd)) {
        Serial.println("Successfull detect I2C RTC") ;
        /*  if (!rtc.isrunning()) {
        Serial.println("RTC is NOT running, setting the time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to the time the sketch was compiled (UTC)
        } */
    } else {
        Serial.println("Couldn't find RTC") ;
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.println("BYPASS ") ;
    }
    // // //
    lcd.begin(16, 2, i2clcd) ;
    lcd.setBacklight(255) ;
    lcd.clear() ;
    // // //
    if(aht.begin(&ahtbmp)) {
        Serial.println("Successfull detect I2C Aht10") ;
    } else {
        Serial.print("Could not find AHT? Check wiring ");
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.print("BYPASS ") ;
        delay(1000) ;
    }
    // // //
    if(bmp.begin()) {
        Serial.println("Successfull detect I2C Bmp280") ;
        bmp.setSampling(Adafruit_BMP280::MODE_FORCED,         /* Operating Mode. */
                                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                                Adafruit_BMP280::STANDBY_MS_500) ;/* Standby time. */
    } else {
        Serial.print(F("Could not find BMP? Check wiring "));
        while (!digitalRead(TTP223)) delay(10);
        Serial.print("BYPASS ") ;
        delay(1000) ;
    }
    // // //
    if(wifiManager.autoConnect("AutoConnectAP")) {
       while (!client.connected()) {
            client.setServer(mqtt_broker, mqtt_port);
            client.setCallback(callback);
            String client_id = "esp32-client-";
            client_id += String(WiFi.macAddress()) ;
            Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str()) ;
            if(client.connect(client_id.c_str())) {
                Serial.println("Public EMQX MQTT broker connected") ;
            } else {
                Serial.print("failed with state ") ;
                Serial.println(client.state());
                delay(2000);
            }
        }
        if(client.subscribe(topic)){
            Serial.print("Success full subscribe to Topic : ");
            Serial.println(topic);
        } 
    } else {
        delay(2000);
        ESP.restart();
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    if(millis() - ploop0 >= loop0) {
        ploop0 = millis() ;
    }
    if(millis() - ploop1 >= loop1) {
        ploop1 = millis() ;
    }
    if(millis() - ploop2 >= loop2) {
        ploop2 = millis() ;
    }
    wifiManager.process();
    client.loop();
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