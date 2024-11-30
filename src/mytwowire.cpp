#include "gheader.h"
#include "gvalue.h"
#include "mytwowire.h"

TwoWire ahtbmp = TwoWire(0) ;
TwoWire i2clcd = TwoWire(1) ;

void mytwowiresetup() {
    ahtbmp.begin(ahtbmpsda, ahtbmpscl) ;
    #ifdef debug
        Serial.println("Successfull create ahtbmp i2c bus");
    #endif
    i2clcd.begin(i2clcdsda, i2clcdscl) ;
    #ifdef debug
        Serial.println("Successfull create i2clcd i2c bus");
    #endif
}