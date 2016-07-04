#include <SHT2x.h>              // Temp/humidity sensor
#include "common.h"

SHT2x tempSensor(0x40); 

bool initTempSensor(void) {
  currentTemp = tempSensor.GetTemperature();
  if ((currentTemp < -50) || (currentTemp > 100)) {
      return false; // check sanity    
  }
  return true;
}

void readTempSensor() {
  currentTemp = tempSensor.GetTemperature();
  currentHum = tempSensor.GetHumidity();
}

