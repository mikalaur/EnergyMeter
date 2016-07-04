#include <INA219.h>
#include "common.h"

INA219 ina219[4];
//INA219 ina219_solar(INA219::I2C_ADDR_41);
//INA219 ina219_alt(INA219::I2C_ADDR_42);
//INA219 ina219_mains(INA219::I2C_ADDR_43);

bool initPowerSensor_Load() {
  ina219[Load].setAddr(0x42);
  ina219[Load].begin();
  if (!ina219[Load].detect()) return false;
  ina219[Load].configure(INA219::RANGE_16V, INA219::GAIN_2_80MV, INA219::ADC_128SAMP, INA219::ADC_128SAMP, INA219::CONT_SH_BUS);
  ina219[Load].calibrate(0.000735, 0.075, 16.0, 100.0);  
  return true;
}

bool initPowerSensor_Solar() {
  ina219[Solar].setAddr(0x41);
  ina219[Solar].begin();
  if (!ina219[Solar].detect()) return false;
  ina219[Solar].configure(INA219::RANGE_32V, INA219::GAIN_4_160MV, INA219::ADC_128SAMP, INA219::ADC_128SAMP, INA219::CONT_SH_BUS);
  ina219[Solar].calibrate(0.0100, 0.100, 32.0, 10.0);  
                        //0.01002
  return true;                       
}

bool initPowerSensor_Alt() {
  ina219[Alt].setAddr(0x44);
  ina219[Alt].begin();
  if (!ina219[Alt].detect()) return false;
  ina219[Alt].configure(INA219::RANGE_16V, INA219::GAIN_2_80MV, INA219::ADC_128SAMP, INA219::ADC_128SAMP, INA219::CONT_SH_BUS);
  ina219[Alt].calibrate(0.00075, 0.075, 16.0, 100.0);  
  return true;
}

bool initPowerSensor_Mains() {
  ina219[Mains].setAddr(0x45);
  ina219[Mains].begin();
  if (!ina219[Mains].detect()) return false;
  ina219[Mains].configure(INA219::RANGE_16V, INA219::GAIN_2_80MV, INA219::ADC_128SAMP, INA219::ADC_128SAMP, INA219::CONT_SH_BUS);
  ina219[Mains].calibrate(0.0100, 0.100, 16.0, 10.0);  
  return true;
}

void readPowerSensors() {
  static float s = 0;
  for (int i = 0; i < 4; i++) {
    powerSensor[i].v = ina219[i].busVoltage();
    powerSensor[i].i = ina219[i].shuntCurrent();
    powerSensor[i].p = ina219[i].busPower();  
  }
}

