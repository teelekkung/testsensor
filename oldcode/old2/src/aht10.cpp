// src/aht10.cpp
#include "globals.h"
#include "aht10.h"

void aht10Task(void *parameter) {
  
  for (;;) {
    SensorData data ;
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    data.ahtTemp = temp.temperature ;
    data.ahtHumi = humidity.relative_humidity ;
    int value = analogRead(TEMP6000) ;
    data.analogValue = value ;
    xQueueSend(sensorDataQueue, &data, portMAX_DELAY);
    Serial.print("AHT20temp: "); Serial.print(temp.temperature); Serial.println(" *C");
    Serial.print("AHT20humi: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
    delay(2000);
  }
}