#include "gheader.h"
#include "gvalue.h"
#include "gfuntion.h"

unsigned long ploop1 = 0 ;
const long loop1 = 20 ;
unsigned long ploop2 = 0 ;
const long loop2 = 2000 ;

void setup() {
    //put the code that run onece
    Serial.begin(115200) ;
    pinMode(TTP223, INPUT) ;
    pinMode(TEMP6000, INPUT) ;
    //setup i2crtc
    mytwowireinit() ;
    i2cdisplayinit() ;
    i2crtcinit() ;
    rgbstripinit() ;
    aht10init() ;
}

void loop() {
    //put the code that run infinitly
    if(millis() - ploop1 >= loop1) {
        ploop1 = millis() ;
        rgbstriprun() ;
    }
    if(millis() - ploop2 >= loop2) {
        ploop2 = millis() ;
        aht10run() ;
    }
}