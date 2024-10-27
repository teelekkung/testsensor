// scr/temp.cpp
#include "globals.h"
#include "temp6000.h"

void temp6000Task(void *parameter) {
    
    for(;;) {
        int value = analogRead(TEMP6000) ;
        Serial.println() ;
        Serial.print("temt6000  : ") ;
        Serial.println(value) ;
    delay(2000) ;
  }
}
