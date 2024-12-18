// scr/ledpwm.cpp
#include "globals.h"
#include "ledpwm.h"

void ledpwmTask(void *parameter) {
  uint16_t k ;
  int state = 0 ;
  for(;;) {
    if(state == 1) {
      if(k < 256) {
        analogWrite(LED_PWM, k) ;
      }
      k += 1 ;
      if(k == 256){
        state = 0 ;
        k = 1 ;
      }
    } else {
      if(k < 256) {
        analogWrite(LED_PWM, 255 - k) ;
      }
      k += 1 ;
      if(k == 256){
        state = 1 ;
        k = 1 ;
      }
    }
    delay(10) ;
  }
}