#include "global.h"
#include "i2cdisplay.h"
#include "rgbstrip.h"
// #define DEBUG

uint16_t i, j = 0 ;
byte *c ;

WiFiManager wifiManager;
TwoWire ahtbmp = TwoWire(0) ;
TwoWire i2clcd = TwoWire(1) ;
RTC_DS1307 rtc;
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp(&ahtbmp);
CRGB leds[RGB_NUMS];
LiquidCrystal_PCF8574 lcd(i2clcdaddr);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200) ;
    // setup TwoWire instants
    ahtbmp.begin(ahtbmpsda, ahtbmpscl) ;
    i2clcd.begin(i2clcdsda, i2clcdscl) ;

    // Setup I2C lcd
    i2cdisplayinit() ;
    // Setup RTC DS1307
    lcd.clear() ;
    displayCenteredText("Init RTC", 0) ;
    if (!rtc.begin(&i2clcd)) {
    Serial.println("Couldn't find RTC");
    while (1); // Stop execution if RTC is not found
    }
    // Check if RTC is running
    if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, setting the time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to the time the sketch was compiled (UTC)
    }
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
    lcd.clear() ;
    // Setup TTP223
    displayCenteredText("Init TTP223", 0) ;
    pinMode(TTP223, INPUT) ;
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
    lcd.clear() ;
    // Setup TEMP6000
    displayCenteredText("Init TEMP6000", 0) ;
    pinMode(TEMP6000, INPUT) ;
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
    lcd.clear() ;
    // Setup 8 bit 2812 RGB
    displayCenteredText("Init LED", 0) ;
    FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, RGB_NUMS).setCorrection(TypicalLEDStrip) ;
    FastLED.setBrightness(255) ;
    FastLED.clear() ;
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
    lcd.clear() ;
    // Setup AHT20
    displayCenteredText("Init AHT20", 0) ;
    if (! aht.begin(&ahtbmp)) {
      Serial.print("Could not find AHT? Check wiring ");
      while (!digitalRead(TTP223)) delay(10) ;
      Serial.print("BYPASS ") ;
      displayCenteredText("BYPASS", 1) ;
    }
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
    lcd.clear() ; 
    // Setup BMP280
    displayCenteredText("Init Bmp280", 0) ;
    if (!bmp.begin()) {
      Serial.print(F("Could not find BMP? Check wiring "));
      while (!digitalRead(TTP223)) delay(10);
      Serial.print("BYPASS ") ;
      displayCenteredText("BYPASS", 1) ;
    }
    bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1000); /* Standby time. */
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
    lcd.clear() ;
    // Setup Wifi For MQTT
    displayCenteredText("Init WIFI", 0) ;
    delay(1000) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
}

void loop() {
  rgbstrip() ;
}



