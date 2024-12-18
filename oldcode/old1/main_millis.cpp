#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

#define ttp223    5
#define RGB_NUMS  9    // Enter the total number of LEDs on the strip
#define RGB_PIN   4    // The pin connected to Din to control the LEDs
#define temt6000  1
#define LEDpin    2

unsigned long ploop1 = 0 ;
const long loop1 = 20 ;
unsigned long ploop2 = 0 ;
const long loop2 = 2000 ;
unsigned long ploop3 = 0 ;
const long loop3 = 10 ;

byte *c ;
uint16_t i, j ;
uint16_t k ;
int state = 0;

byte *Wheel(byte WheelPosition) ;
void rainbowCycle() ;
void aht20() ;
void bmp280() ;
void light6000() ;
void pulseled() ;

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;
CRGB leds[RGB_NUMS] ;

void setup() {
  Serial.begin(115200) ;
  Serial.println("Esp32s3 Setup ...") ;
  Serial.print("Starting up ttp223 ... ") ;
  pinMode(ttp223, INPUT) ;
  delay(1000);
  Serial.println("Done") ;
  Serial.print("Starting up FastLED WS2812 ... ") ;
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, RGB_NUMS).setCorrection(TypicalLEDStrip) ;
  FastLED.clear() ;
  delay(1000);
  Serial.println("Done") ;
  Serial.print("Starting up Adafruit AHT20 ... ");
  if (! aht.begin()) {
    Serial.print("Could not find AHT? Check wiring ");
    while (!digitalRead(ttp223)) delay(10) ;
    Serial.print("BYPASS ") ;
  }
  delay(1000) ;
  Serial.println("Done");
  Serial.print("Starting up Adafruit BMP280 ... ");
  if (!bmp.begin()) {
    Serial.print(F("Could not find BMP? Check wiring "));
    while (!digitalRead(ttp223)) delay(10);
    Serial.print("BYPASS ") ;
  }
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  delay(1000) ;
  Serial.println("Done");

  Serial.println("Starting void loop ...");
  Serial.println();
}

void loop() {
  if(millis() - ploop1 >= loop1) {
    ploop1 = millis() ;
    rainbowCycle() ;
  }
  if(millis() - ploop2 >= loop2) {
    ploop2 = millis() ;
    aht20() ;
    light6000() ;
    bmp280() ;
    Serial.println();
    float temp_celsius = temperatureRead();
    Serial.print("Temp onBoard ");
    Serial.print(temp_celsius);
    Serial.println("°C");
  }
  if(millis() - ploop3 >= loop3) {
    ploop3 = millis() ;
    pulseled() ;
  }
}


void rainbowCycle() {
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

void aht20() {
  sensors_event_t humidity, temp ;
  aht.getEvent(&humidity, &temp) ;// populate temp and humidity objects with fresh data
  Serial.print("AHT20temp : "); Serial.print(temp.temperature); Serial.println(" *C") ;
  Serial.print("AHT20humi : "); Serial.print(humidity.relative_humidity); Serial.println("% rH") ;
}

void bmp280() {
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
}

void light6000(){
  int value = analogRead(temt6000);
  Serial.print("temt6000  : ");
  Serial.println(value);
}

void pulseled(){
  if(state == 1) {
    if(k < 256) {
      analogWrite(LEDpin, k);
    }
    k += 1 ;
    if(k == 256){
      state = 0 ;
      k = 1 ;
    }
  } else {
    if(k < 256) {
      analogWrite(LEDpin, 255 - k);
    }
    k += 1 ;
    if(k == 256){
      state = 1 ;
      k = 1 ;
    }
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