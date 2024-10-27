#include "globals.h"
#include "rgbstrip.h"
#include "ledpwm.h"
#include "temp6000.h"
#include "aht10.h"
#include "bmp280.h"
#include "cputemp.h"

TaskHandle_t rgbstripHand, ledpwmHand, temp6000Hand, aht10Hand, bmp280Hand, cputempHand ;
QueueHandle_t queue;
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;
CRGB leds[RGB_NUMS];
int bootstatus = 0 ;

void setup() {
  // setup RTOS
  Serial.begin(115200) ;
  Serial.println();
  Serial.println("Esp32s3 Booting up") ;
  Serial.println();
  delay(1000);

  Serial.print("Starting up TTP223 ... ") ;
  pinMode(TTP223, INPUT) ;
  delay(1000);
  Serial.println("Done") ;
  delay(1000);

  Serial.print("Starting up FastLED WS2812 ... ") ;
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, RGB_NUMS).setCorrection(TypicalLEDStrip) ;
  FastLED.clear() ;
  delay(1000);
  Serial.println("Done") ;
  delay(1000);

  Serial.print("Starting up Adafruit AHT20 ... ");
  if (! aht.begin()) {
    Serial.print("Could not find AHT? Check wiring ");
    while (!digitalRead(TTP223)) delay(10) ;
    Serial.print("BYPASS ") ;
  }
  delay(1000) ;
  Serial.println("Done");
  delay(1000);

  Serial.print("Starting up Adafruit BMP280 ... ");
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
  delay(1000) ;
  Serial.println("Done") ;
  delay(1000) ;
  
  Serial.println("Starting Task ... ") ;
  Serial.println() ;
  xTaskCreate( temp6000Task,  "temp6000", 1500, NULL, 1, &temp6000Hand  ) ;
  xTaskCreate( rgbstripTask,  "rgbstrip", 1500, NULL, 1, &rgbstripHand  ) ;
  xTaskCreate( cputempTask,   "cpu_temp", 1500, NULL, 1, &cputempHand   ) ;
  xTaskCreate( ledpwmTask,    "ledpwm",   1500, NULL, 1, &ledpwmHand    ) ;
  xTaskCreate( aht10Task,     "aht10",    3000, NULL, 1, &aht10Hand     ) ;
  xTaskCreate( bmp280Task,    "bpm280",   3000, NULL, 1, &bmp280Hand    ) ;  
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
