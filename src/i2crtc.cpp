#include "gheader.h"
#include "gvalue.h"
#include "i2crtc.h"

RTC_DS1307 rtc;

void i2crtcinit() {
    #ifdef debug
        Serial.println("Starting I2C RTC Instant") ;
    #endif
    displayCenteredText("Setup RTC", 0) ;
    if (!rtc.begin(&i2clcd)) {
        Serial.println("Couldn't find RTC") ;
        while (!digitalRead(TTP223)) delay(10) ;
        Serial.println("BYPASS ") ;
        delay(1000) ;
        displayCenteredText("Bypass", 1) ;
    } else {
        #ifdef debug
            Serial.println("Successfull detect I2C RTC") ;
        #endif
        delay(1000) ;
        displayCenteredText("Done", 1) ;
    }
/*     if (!rtc.isrunning()) {
        Serial.println("RTC is NOT running, setting the time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to the time the sketch was compiled (UTC)
    } */
    delay(1000) ;
    lcd.clear() ;
}