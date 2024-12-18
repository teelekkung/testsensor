#include "gheader.h"
#include "gvalue.h"
#include "i2crtc.h"

RTC_DS1307 rtc;
const int TIMEZONE_OFFSET = 7; // GMT+7
bool lcdInitialized = false;
int var = 0;

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

void i2crtcrun() {
    DateTime now ;
    now = rtc.now() ;
    char timeBuffer[17];
    char dateBuffer[17];
    DateTime localTime = now + TimeSpan(TIMEZONE_OFFSET * 3600);
    snprintf(timeBuffer, sizeof(timeBuffer), "Time: %02d:%02d:%02d", localTime.hour(), localTime.minute(), localTime.second());
    snprintf(dateBuffer, sizeof(dateBuffer), "Date: %02d/%02d/%04d", localTime.day(), localTime.month(), localTime.year());
    displayCenteredText(timeBuffer, 0);
    displayCenteredText(dateBuffer, 1);
}