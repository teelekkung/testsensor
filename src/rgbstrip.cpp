#include "gheader.h"
#include "gvalue.h"
#include "rgbstrip.h"

CRGB leds[RGB_NUMS];
uint16_t i, j, k = 0 ;
byte *c ;
int state = 0;

void rgbstripinit() {
    #ifdef debug
        Serial.println("Starting RGB Instant");
    #endif
    displayCenteredText("Setup RGB", 0) ;
    FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, RGB_NUMS).setCorrection(TypicalLEDStrip) ;
    FastLED.setBrightness(255) ;
    FastLED.clear() ;
    #ifdef debug
        Serial.println("Successfull create RGB Instant") ;
    #endif
    delay(1000) ;
    for(int i = 0; i < RGB_NUMS; i++) {
        leds[i].setRGB(0, 0, 0) ;
    }
    FastLED.show() ;
    displayCenteredText("Done", 1) ;
    delay(1000);
    lcd.clear();
}

void rgbstriprun() {
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
}

void pulseled(){
  if(state == 1) {
    if(k < 256) {
      analogWrite(LED_PWM, k);
    }
    k += 1 ;
    if(k == 256){
      state = 0 ;
      k = 1 ;
    }
  } else {
    if(k < 256) {
      analogWrite(LED_PWM, 255 - k);
    }
    k += 1 ;
    if(k == 256){
      state = 1 ;
      k = 1 ;
    }
  } 
}

byte *Wheel(byte WheelPosition) {
    static byte c[3];
    if(WheelPosition < 85) {
        c[0] = WheelPosition * 3;
        c[1] = 255 - WheelPosition * 3;
        c[2] = 0;
    } else if (WheelPosition < 170) {
        WheelPosition -= 85;
        c[0] = 255 - WheelPosition * 3;
        c[1] = 0;
        c[2] = WheelPosition * 3;
    } else  {
        WheelPosition -= 170;
        c[0] = 0;
        c[1] = WheelPosition * 3;
        c[2] = 255 - WheelPosition * 3;
    }
    return c;
}
