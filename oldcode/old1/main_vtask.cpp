#include "globals.h"
#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

//#define AHT10     sda8, scl9
//#define BMP280    sda8, scl9
#define TEMP6000    1
#define LED_BUILTIN 2
#define TTP223      5
//FastLED
#define RGB_NUMS  9
#define RGB_PIN   4
#define debugg

int led1 = 40;
int led2 = 41;
int led3 = 42;

TaskHandle_t TAHT10, TBMP280, TFASTLED, TTEMP6000, TLED_BUILTIN, TCUP_TEMP ;
QueueHandle_t queue;
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;
CRGB leds[RGB_NUMS] ;

void Faht10(void *parameter) ;
void Fbmp280(void *parameter) ;
void Ffastled(void *parameter) ;
void Ftemp6000(void *parameter) ;
void Fled_buildin(void *parameter) ;
void Fcpu_temp(void *parameter) ;
byte *Wheel(byte WheelPosition) ;
int bootstatus = 0 ;

void setup() {
  // setup RTOS
  Serial.begin(115200) ;
  Serial.println();
  Serial.println("Esp32s3 Booting up") ;
  Serial.println();
  delay(1000);

  Serial.println("Starting up TTP223 ... ") ;
  pinMode(TTP223, INPUT) ;
  xTaskCreate(
    Ftemp6000, 
    "temp6000", 
    1500, 
    NULL, 
    1, 
    &TTEMP6000
  );
  delay(1000);
  Serial.println("Done") ;
  delay(1000);

  Serial.println("Starting up FastLED WS2812 ... ") ;
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, RGB_NUMS).setCorrection(TypicalLEDStrip) ;
  FastLED.clear() ;
  xTaskCreate(
    Ffastled, 
    "FastLED", 
    1500, 
    NULL, 
    1, 
    &TFASTLED
  );
  xTaskCreate(
    Fcpu_temp, 
    "cpu_temp", 
    1500, 
    NULL, 
    1, 
    &TCUP_TEMP
  );
  xTaskCreate(
    Fled_buildin, 
    "led_builtin", 
    1500, 
    NULL, 
    1, 
    &TLED_BUILTIN
  );
  delay(1000);
  Serial.println("Done") ;
  delay(1000);

  Serial.println("Starting up Adafruit AHT20 ... ");
  if (! aht.begin()) {
    Serial.print("Could not find AHT? Check wiring ");
    while (!digitalRead(TTP223)) delay(10) ;
    Serial.print("BYPASS ") ;
  }
  xTaskCreate(
    Faht10, 
    "aht10", 
    3000, 
    NULL, 
    1, 
    &TAHT10
  );
  delay(1000) ;
  Serial.println("Done");
  delay(1000);

  Serial.println("Starting up Adafruit BMP280 ... ");
  if (!bmp.begin()) {
    Serial.print(F("Could not find BMP? Check wiring "));
    while (!digitalRead(TTP223)) delay(10);
    Serial.print("BYPASS ") ;
  }
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  xTaskCreate(
    Fbmp280, 
    "bpm280", 
    3000, 
    NULL, 
    1, 
    &TBMP280
  );  
  delay(1000) ;
  Serial.println("Done");

  bootstatus = 1 ;
}

void loop(){
  // Just Nothing~~
  /*
  UBaseType_t taskaht10 = uxTaskGetStackHighWaterMark(TAHT10);
  Serial.print("Taht10 : ");
  Serial.println(taskaht10) ;
  UBaseType_t taskbmp280 = uxTaskGetStackHighWaterMark(TBMP280);
  Serial.print("TBMP280 : ");
  Serial.println(taskbmp280) ;
  delay(500) ;
  */
}

void Faht10(void *parameter) {
  for(;;) {
    if(bootstatus == 1) {
      sensors_event_t humidity, temp ;
      aht.getEvent(&humidity, &temp) ;// populate temp and humidity objects with fresh data
      Serial.print("AHT20temp : "); Serial.print(temp.temperature); Serial.println(" *C") ;
      Serial.print("AHT20humi : "); Serial.print(humidity.relative_humidity); Serial.println("% rH") ;
    }
    delay(2000) ;
  }
}
void Fbmp280(void *parameter) {
  for(;;) {
    if(bootstatus == 1) {
      if (bmp.takeForcedMeasurement()) {
        // can now print out the new measurements
        Serial.print(F("BMP280temperature = ")) ;
        Serial.print(bmp.readTemperature()) ;
        Serial.println(" *C") ;

        Serial.print(F("BMP280pressure    = ")) ;
        Serial.print(bmp.readPressure()) ;
        Serial.println(" Pa") ;

        Serial.print(F("BMP280altitude    = ")) ;
        Serial.print(bmp.readAltitude(1010.25)) ; /* Adjusted to local forecast! */
        Serial.println(" m") ;
      } else {
        Serial.println("Forced measurement failed!") ;
      }
    }
    delay(2000) ;
  }
}
void Ffastled(void *parameter) {
  uint16_t i, j ;
  byte *c ;
  for(;;){
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
    delay(20) ;
  }
}
void Ftemp6000(void *parameter) {
  for(;;) {
    if(bootstatus == 1) {
      int value = analogRead(TEMP6000) ;
      Serial.println() ;
      Serial.print("temt6000  : ") ;
      Serial.println(value) ;
    }
    delay(2000) ;
  }
}
void Fled_buildin(void *parameter) {
  uint16_t k ;
  int state = 0 ;
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  for(;;) {
    if(state == 1) {
      if(k < 256) {
        analogWrite(LED_BUILTIN, k) ;
      }
      k += 1 ;
      if(k == 256){
        state = 0 ;
        k = 1 ;
      }
    } else {
      if(k < 256) {
        analogWrite(LED_BUILTIN, 255 - k) ;
      }
      k += 1 ;
      if(k == 256){
        state = 1 ;
        k = 1 ;
      }
    }
    delay(10) ;
  }
}
void Fcpu_temp(void *parameter) {
  int i = 1 ;
  for(;;){
    if(bootstatus == 1) {
      float temp_celsius = temperatureRead();
      Serial.print("Temp onBoard ");
      Serial.print(temp_celsius);
      Serial.println("°C");
    }
    if(i > 8) {
      i = 0 ;
    } 
    digitalWrite(led3, bitRead(i,2));
    digitalWrite(led2, bitRead(i,1));
    digitalWrite(led1, bitRead(i,0));
    i++ ;
    
    delay(2000) ;
  }
}
byte *Wheel(byte WheelPosition) {
  static byte c[3];
 
  if(WheelPosition < 85) {
   c[0] = WheelPosition * 3;
   c[1] = 255 - WheelPosition * 3;
   c[2] = 0;
  }
  else if(WheelPosition < 170) {
   WheelPosition -= 85;
   c[0] = 255 - WheelPosition * 3;
   c[1] = 0;
   c[2] = WheelPosition * 3;
  }
  else {
   WheelPosition -= 170;
   c[0] = 0;
   c[1] = WheelPosition * 3;
   c[2] = 255 - WheelPosition * 3;
  }

  return c;
}