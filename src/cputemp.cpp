// scr/cputemp.cpp
#include "globals.h"
#include "cputemp.h"

void cputempTask(void *parameter) {
  int i = 1 ;
  pinMode(led1, OUTPUT) ;
  pinMode(led2, OUTPUT) ;
  pinMode(led3, OUTPUT) ;
  for(;;){
    float temp_celsius = temperatureRead() ;
    Serial.print("Temp onBoard ") ;
    Serial.print(temp_celsius) ;
    Serial.println("°C") ;
    if(i > 8) {
      i = 1 ;
    } 
    digitalWrite(led3, bitRead(i,2)) ;
    digitalWrite(led2, bitRead(i,1)) ;
    digitalWrite(led1, bitRead(i,0)) ;
    i = i + 1 ;
    
    delay(2000) ;
  }
}