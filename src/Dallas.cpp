/*
Copyright of Sanjay Bhatikar
*/

#include "Dallas.h"

Dallas::Dallas() {

}

void Dallas::start(int signal) {
    this->_wire.begin(signal);
    this->_ds18b20.setOneWire(&this->_wire);
    this->_ds18b20.begin();
}

float Dallas::get_Temp(enum TemperatureUnit unit, bool live) {
    float retval = -99.99;
    if (!live) {
        if (unit==CELSIUS) {
            retval= this->_TempC;
        } else if (unit==FAHRENHEIT) {
            retval= this->_TempF;
        }
    } else {
        this->_TempC = this->_ds18b20.getTempCByIndex(0);
        this->_TempF = this->convertC2F(this->_TempC);
        if (unit==CELSIUS) {
            retval= this->_TempC;
        } else if (unit==FAHRENHEIT) {
            retval= this->_TempF;
        }
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