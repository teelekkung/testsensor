// scr/rgbstrip.cpp
#include "globals.h"
#include "rgbstrip.h"
#include <FastLED.h>

byte *Wheel(byte WheelPosition) ;

void rgbstripTask(void *parameter) {
  uint16_t i, j ;
  byte *c ;
  for(;;){
    if(j < 256) {
      for(i=0; i < RGB_NUMS; i++) {
        c = Wheel(((i * 256 / RGB_NUMS) + j) & 255) ;
        leds[RGB_NUMS - 1 - i].setRGB(*c, *(c+1), *(c+2)) ;
      }
      FastLED.show() ;
    }
    j++ ;
    if(j == 256) {
      j = 0 ;
    }
    delay(20) ;
  }
}

byte *Wheel(byte WheelPosition) {
  static byte c[3];
 
  if(WheelPosition < 85) {
   c[0] = WheelPosition * 3;
   c[1] = 255 - WheelPosition * 3;
   c[2] = 0;
  }
  else if(WheelPosition < 170) {
   WheelPosition -= 85;
   c[0] = 255 - WheelPosition * 3;
   c[1] = 0;
   c[2] = WheelPosition * 3;
  }
  else {
   WheelPosition -= 170;
   c[0] = 0;
   c[1] = WheelPosition * 3;
   c[2] = 255 - WheelPosition * 3;
  }

  return c;
}