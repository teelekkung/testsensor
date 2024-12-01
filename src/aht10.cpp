#include "gheader.h"
#include "gvalue.h"
#include "aht10.h"

Adafruit_AHTX0 aht;

void aht10init() {
    #ifdef debug
        Serial.println("Starting I2C Aht10 Instant") ;
    #endif
    displayCenteredText("Setup Aht10", 0) ;
    if (! aht.begin(&ahtbmp)) {
        Serial.print("Could not find AHT? Check wiring ");
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.print("BYPASS ") ;
        delay(1000) ;
        displayCenteredText("Bypass", 1) ;
    } else {
        #ifdef debug
            Serial.println("Successfull detect I2C Aht10") ;
        #endif
        delay(1000) ;
        displayCenteredText("Done", 1) ;
    }
    delay(1000) ;
    lcd.clear() ;
}

void aht10run(){
    sensors_event_t humidity, temp ;
    aht.getEvent(&humidity, &temp) ;// populate temp and humidity objects with fresh data
    Serial.print("AHT20temp : "); Serial.print(temp.temperature); Serial.println(" *C") ;
    Serial.print("AHT20humi : "); Serial.print(humidity.relative_humidity); Serial.println("% rH") ;
}