// src/bmp280.cpp
#include "globals.h"
#include "bmp280.h"

void bmp280Task(void *parameter) {
  
  for(;;) {
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
    delay(2000) ;
  }
}
