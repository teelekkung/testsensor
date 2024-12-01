#include "gheader.h"
#include "gvalue.h"
#include "bmp280.h"

Adafruit_BMP280 bmp(&ahtbmp);

void bmp280init() {
    #ifdef debug
        Serial.println("Starting I2C Bmp290 Instant") ;
    #endif
    displayCenteredText("Setup Bmp280", 0) ;
    if (!bmp.begin()) {
        Serial.print(F("Could not find BMP? Check wiring "));
        while (!digitalRead(TTP223)) delay(10);
        Serial.print("BYPASS ") ;
        delay(1000) ;
        displayCenteredText("Bypass", 1) ;
    } else {
        bmp.setSampling(Adafruit_BMP280::MODE_FORCED,   /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
        #ifdef debug
            Serial.println("Successfull detect I2C Bmp280") ;
        #endif
        delay(1000) ;
        displayCenteredText("Done", 1) ;
    }
    delay(1000) ;
    lcd.clear() ;

}

void bmp280run() {
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
