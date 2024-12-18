// include/rgbstrip.h
#ifndef RGBSTRIP_H
    #define RGBSTRIP_H

    #ifndef Arduino_h
        #include <Arduino.h>
    #endif
    
    extern uint16_t i, j ;
    extern byte *c ;
    extern void rgbstrip() ;
    extern byte *Wheel(byte WheelPosition) ;

#endif