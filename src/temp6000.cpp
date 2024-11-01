// scr/temp.cpp
#include "globals.h"
#include "temp6000.h"

void temp6000Task(void *parameter) {
  pinMode(relay, OUTPUT) ;
  bool state = true ;
  for(;;) { ;
    int value = analogRead(TEMP6000) ;
    Serial.println() ;
    Serial.print("temt6000  : ") ;
    Serial.println(value) ;
    if(state == true) {
      digitalWrite(relay, HIGH) ;
      delay(1000) ;
      digitalWrite(relay, LOW) ;
      delay(1000) ;
    } else {
      delay(2000) ;
    }
    if(digitalRead(TTP223) == 1){
      state = !state ;
      Serial.println("Turn off relay");
    }
  }
}
