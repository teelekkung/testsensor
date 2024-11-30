#ifndef gvalue_h
    #define gvalue_h
        #ifndef gheader_h
            #define gheader_h
        #endif

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

    #define debug

    extern WiFiManager wifiManager;
    extern TwoWire ahtbmp ;
    extern TwoWire i2clcd ;
    extern RTC_DS1307 rtc;
    extern Adafruit_AHTX0 aht ;
    extern Adafruit_BMP280 bmp ;
    extern CRGB leds[RGB_NUMS];
    extern LiquidCrystal_PCF8574 lcd ;
    extern WiFiClient espClient ;
    extern PubSubClient client ;

#endif