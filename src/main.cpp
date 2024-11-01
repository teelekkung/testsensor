#include "globals.h"
#include "rgbstrip.h"
#include "ledpwm.h"
#include "temp6000.h"
#include "aht10.h"
#include "bmp280.h"
#include "cputemp.h"
#include "wifimanaged.h"
#include "i2cdeiplay.h"

TaskHandle_t rgbstripHand, ledpwmHand, temp6000Hand, aht10Hand, bmp280Hand, cputempHand , wifimanagedHand, i2cdisplayHand;
QueueHandle_t sensorDataQueue;
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;
CRGB leds[RGB_NUMS];
LiquidCrystal_PCF8574 lcd(0x25);
TwoWire I2C_LCD = TwoWire(1);
int bootstatus = 0 ;

void setup() {

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
  FastLED.setBrightness(32) ;
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

  Serial.print("Starting up I2C display ... ") ;
  I2C_LCD.begin(38, 39, 100000);
  lcd.begin(16, 2, I2C_LCD) ;
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print("Sahakiyat Sensor");
  delay(1000) ;
  Serial.println("Done") ;
  delay(1000) ;

  sensorDataQueue = xQueueCreate(4, sizeof(SensorData));
  if (sensorDataQueue == NULL) {
        Serial.println("Error creating the queue");
        while (true); // Loop forever if the queue was not created
  }

  Serial.println("Starting Task ... ") ;
  Serial.println() ;
  xTaskCreate( rgbstripTask,  "rgbstrip", 1500, NULL, 1, &rgbstripHand  ) ;
  xTaskCreate( ledpwmTask,    "ledpwm",   1500, NULL, 1, &ledpwmHand    ) ;
  xTaskCreate( temp6000Task,  "temp6000", 1500, NULL, 1, &temp6000Hand  ) ;
  xTaskCreate( cputempTask,   "cpu_temp", 1500, NULL, 1, &cputempHand   ) ;
  xTaskCreate( aht10Task,     "aht10",    3000, NULL, 1, &aht10Hand     ) ;
  xTaskCreate( bmp280Task,    "bpm280",   3000, NULL, 1, &bmp280Hand    ) ;
  xTaskCreate( i2cdisplayTask,"i2cdisplay",3000,NULL, 1, &i2cdisplayHand) ;
  // xTaskCreatePinnedToCore( wifimanagedTask, "wifimanaged", 10000, NULL, 1, &wifimanagedHand , 0) ;
}

void loop(){
  // Just Nothing~~
  /*
  UBaseType_t i2cdisplayram = uxTaskGetStackHighWaterMark(i2cdisplayHand);
  Serial.print("i2cdisplayTask : ");
  Serial.print(xPortGetCoreID());
  Serial.print(" raam ");
  Serial.println(i2cdisplayram) ;
  delay(1000) ;
  */
}
