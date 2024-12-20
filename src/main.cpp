#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_SSD1306.h>
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
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

void callback(char *topic, byte *payload, unsigned int length) ;
void dotemp() ;
void doaht() ;
void dobmp() ;
void dorgb() ;
byte *Wheel(byte WheelPosition) ;
void dortc() ;
void doled() ;
void dopwm() ;
void docpu() ;

const int TIMEZONE_OFFSET = 7 ;
const char *mqtt_broker = "10.42.0.1";
const int mqtt_port = 1883;
const char *topic = "ALERT";
const char *datadsent ;
uint16_t i, j, k, m = 0 ;
int l, direction = 1 ;
byte *c ;
int state = 0;
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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &i2clcd, OLED_RESET);
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
    // SYSTEM //
    pinMode(TEMP6000, INPUT) ;
    pinMode(TTP223, INPUT) ;
    pinMode(LED_PWM, OUTPUT) ;
    pinMode(12, OUTPUT) ;
    pinMode(ledbit1, OUTPUT) ;
    pinMode(ledbit2, OUTPUT) ;
    pinMode(ledbit3, OUTPUT) ;
    // TwoWire //
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
    // RTCds1307 //
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
    // I2C LCD 12x6 //
    lcd.begin(16, 2, i2clcd) ;
    lcd.setBacklight(255) ;
    lcd.clear() ;
    lcd.print("66200408 I2C DEV") ;
    lcd.setCursor(0,1) ;
    lcd.print("Sahakiat Teelek") ;
    // I2C OLED //
    if(display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("Successfull detect I2C OLED") ;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.cp437(true);
        display.println("I2COLED By\n66200408") ;
        display.display();
        display.clearDisplay();
        delay(2000);
    } else {
        Serial.println(F("SSD1306 allocation failed"));
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.println("BYPASS ") ;
    }
    // AHT10 //
    if(aht.begin(&ahtbmp)) {
        Serial.println("Successfull detect I2C Aht10") ;
    } else {
        Serial.print("Could not find AHT? Check wiring ");
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.print("BYPASS ") ;
        delay(1000) ;
    }
    // BMP280 //
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
    // 8bit RGB2812 //
    FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, RGB_NUMS).setCorrection(TypicalLEDStrip) ;
    FastLED.setBrightness(255) ;
    FastLED.clear() ;
    for(int i = 0; i < RGB_NUMS; i++) {
        leds[i].setRGB(0, 0, 0) ;
    }
    FastLED.show() ;
    // WiFi,MQTT //
    if(wifiManager.autoConnect("AutoConnectAP")) {
        lcd.clear();
        lcd.print("Got Wifi") ;
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
        lcd.clear() ;
    } else {
        delay(2000);
        ESP.restart();
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    if(millis() - ploop0 >= loop0) {
        ploop0 = millis() ;
        dorgb() ;
        dopwm() ;
    }
    if(millis() - ploop1 >= loop1) {
        ploop1 = millis() ;
        dortc() ;
        doled() ;
    }
    if(millis() - ploop2 >= loop2) {
        ploop2 = millis() ;
        dotemp() ;
        doaht() ;
        dobmp() ;
        docpu() ;
        datadsent = digitalRead(12) == HIGH ? "1" : "0";
        client.publish("Status", datadsent);
        display.clearDisplay();
    }
    wifiManager.process();
    client.loop();
}

void callback(char *topic, byte *payload, unsigned int length) {
    // Serial.print("Message arrived in topic: ");
    // Serial.println(topic);
    // Serial.print("Message:");
    String messagedata;
    for (int i = 0; i < length; i++) {
        messagedata += (char) payload[i];
        //Serial.print((char) payload[i]);
    }
    // Serial.print(messagedata);

    //messagedata = atoi(messagedata);
    // Serial.print("Turn LED ") ;
    if(messagedata == "1"){
      // Serial.println("off");
      digitalWrite(12, LOW);
    } else {
      // Serial.println("on");
      digitalWrite(12, HIGH);
    }
    // Serial.println();
    // Serial.println("-----------------------");
}

void dotemp() {
    int lsv = analogRead(TTP223) ;
    Serial.print("temt6000  : ") ;
    Serial.println(lsv) ;
}

void doaht(){
    sensors_event_t humidity, temp ;
    aht.getEvent(&humidity, &temp) ;// populate temp and humidity objects with fresh data
    Serial.print("AHT20temp : "); Serial.print(temp.temperature); Serial.println(" *C") ;
    Serial.print("AHT20humi : "); Serial.print(humidity.relative_humidity); Serial.println("% rH") ;
    char tempStr[8];
    dtostrf(temp.temperature, 6, 2, tempStr); 
    client.publish("Temp", tempStr);
    display.setCursor(0,0) ;
    display.println(tempStr) ;
    display.display();
}

void dobmp() {   
    if (bmp.takeForcedMeasurement()) {
    // can now print out the new measurements
    Serial.print(F("BMP280temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("BMP280pressure    = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("BMP280altitude    = "));
    Serial.print(bmp.readAltitude(1010.25)); /* Adjusted to local forecast! */
    Serial.println(" m");
    } else {
        Serial.println("Forced measurement failed!");
    }
};

void dorgb() {
    if(j < 256) {
    for(i=0; i < RGB_NUMS; i++) {
        c = Wheel(((i * 256 / RGB_NUMS) + j) & 255) ;
        leds[RGB_NUMS - 1 - i].setRGB(*c, *(c+1), *(c+2)) ;
    }
    FastLED.show() ;
    }
    j++ ;
    if(j == 256) {
    j = 0 ;
    }
}

byte *Wheel(byte WheelPosition) {
    static byte c[3];
    if(WheelPosition < 85) {
        c[0] = WheelPosition * 3;
        c[1] = 255 - WheelPosition * 3;
        c[2] = 0;
    } else if (WheelPosition < 170) {
        WheelPosition -= 85;
        c[0] = 255 - WheelPosition * 3;
        c[1] = 0;
        c[2] = WheelPosition * 3;
    } else  {
        WheelPosition -= 170;
        c[0] = 0;
        c[1] = WheelPosition * 3;
        c[2] = 255 - WheelPosition * 3;
    }
    return c;
}

void dortc() {
    DateTime now ;
    now = rtc.now() ;
    char timeBuffer[17];
    char dateBuffer[17];
    DateTime localTime = now + TimeSpan(TIMEZONE_OFFSET * 3600);
    snprintf(timeBuffer, sizeof(timeBuffer), "Time: %02d:%02d:%02d", localTime.hour(), localTime.minute(), localTime.second());
    snprintf(dateBuffer, sizeof(dateBuffer), "Date: %02d/%02d/%04d", localTime.day(), localTime.month(), localTime.year());
    lcd.setCursor(0,0);
    lcd.print(timeBuffer);
    lcd.setCursor(0,1);
    lcd.print(dateBuffer);
}

void doled() {
    if(l > 8) {
      l = 1 ;
    } 
    digitalWrite(ledbit3, bitRead(l,2)) ;
    digitalWrite(ledbit2, bitRead(l,1)) ;
    digitalWrite(ledbit1, bitRead(l,0)) ;
    l = l + 1 ;
}

void dopwm() {
    // Update the PWM value
    m += direction;
    analogWrite(LED_PWM, m);

    // Change direction at boundaries
    if (m >= 255) {
        direction = -1; // Start decrementing
    } else if (m <= 0) {
        direction = 1; // Start incrementing
    }
}

void docpu() {
    float temp_celsius = temperatureRead() ;
    Serial.print("Temp onBoard      = ") ;
    Serial.print(temp_celsius) ;
    Serial.println("°C") ;
}