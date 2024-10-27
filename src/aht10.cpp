// src/aht10.cpp
#include "globals.h"
#include "aht10.h"

void aht10Task(void *parameter) {
  
  for (;;) {
      sensors_event_t humidity, temp;
      aht.getEvent(&humidity, &temp);
      Serial.print("AHT20temp: "); Serial.print(temp.temperature); Serial.println(" *C");
      Serial.print("AHT20humi: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
    delay(2000);
  }
}