// src/i2cdisplay.cpp
#include "globals.h"
#include "i2cdeiplay.h"

void i2cdisplayTask(void *parameter) {
    lcd.clear() ;
    lcd.setCursor(0,0) ;
    lcd.print("Starting Task") ;
    delay(1000);
    SensorData receivedData;
    lcd.clear() ;
    for(;;){
        xQueueReceive(sensorDataQueue, &receivedData, portMAX_DELAY) ;
        lcd.setCursor(0,0) ;
        lcd.print(receivedData.ahtTemp) ;
        lcd.setCursor(6,0) ;
        lcd.print(receivedData.ahtHumi) ;
        lcd.setCursor(0,1) ;
        lcd.print(String(receivedData.analogValue)+"   ") ;
        delay(1000);
    }
}