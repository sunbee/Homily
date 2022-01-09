/*
Copyright of Sanjay Bhatikar
*/
#ifndef DALLAS_H
#define DALLAS_H

#include <Arduino.h>
#include "Pins.h"

#include <Wire.h>
#include <DallasTemperature.h>

enum TemperatureUnit {
    FAHRENHEIT,
    CELSIUS
};

class Dallas 
{
    public:
        Dallas();
        void start();
        float get_Temp(enum TemperatureUnit=FAHRENHEIT, bool=true);
        float convertC2F(float c);
        float convertF2C(float f);
    private:
        float _TempF;
        float _TempC;
        OneWire _wire = OneWire(PIN_DALLAS);
        DallasTemperature _ds18b20 = DallasTemperature();    
};

#endif