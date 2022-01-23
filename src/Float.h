/*
Copyright of Sanjay Bhatikar
*/
#ifndef FLOAT_H
#define FLOAT_H

#include <Arduino.h>
#include "Pins.h"

class Float
{
    public:
        Float(int);
        bool get_isflood(bool=true);
    private:
        int _pin;
        bool _isflood;
};

#endif