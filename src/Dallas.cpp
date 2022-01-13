/*
Copyright of Sanjay Bhatikar
*/

#include "Dallas.h"

Dallas::Dallas() {

}

void Dallas::start() {
    this->_ds18b20.setOneWire(&this->_wire);
    this->_ds18b20.begin();
    delay(1500);
    this->_ds18b20.requestTemperatures();
}

float Dallas::get_Temp(enum TemperatureUnit unit, bool live) {
    this->_ds18b20.requestTemperatures();
    float retval = -99.99;
    if (live) {
        this->_TempC = this->_ds18b20.getTempCByIndex(0);
        this->_TempF = this->_ds18b20.getTempFByIndex(0);
    }
    if (unit==CELSIUS) {
        retval= this->_TempC;
    } else if (unit==FAHRENHEIT) {
        retval= this->_TempF;
    }
    return retval;
}

float Dallas::convertC2F(float c) 
{
  return (c * 1.8) + 32.0;
}

float Dallas::convertF2C(float f)
{
  return (f - 32.0) / 1.8;
}