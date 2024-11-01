// include globals.h
#ifndef GLOBALS_H
    #define GLOBALS_H

    #include <Arduino.h>
    #include <FastLED.h>
    #include <Adafruit_AHTX0.h>
    #include <Adafruit_BMP280.h>
    #include <WiFiManager.h>
    #include <LiquidCrystal_PCF8574.h>

    // Define pins here
    //#define AHT10     sda8, scl9
    //#define BMP280    sda8, scl9
    #define TEMP6000    4
    // FastLED
    #define RGB_NUMS    9
    #define RGB_PIN     5
    #define TTP223      6
    #define LED_PWM     7
    #define relay       15
    #define led1 16
    #define led2 17
    #define led3 18 

    extern LiquidCrystal_PCF8574 lcd ;
    extern Adafruit_AHTX0 aht;
    extern Adafruit_BMP280 bmp;
    extern CRGB leds[RGB_NUMS];
    extern int bootstatus;
    extern QueueHandle_t sensorDataQueue;
    struct SensorData {
        float ahtTemp ;
        float ahtHumi ;
        int analogValue ;
    };
    extern SensorData sensorDataInstance;

#endif
