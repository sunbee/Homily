/*
Copyright of Sanjay Bhatikar
*/
#ifndef FLAME_H
#define FLAME_H

#include <Arduino.h>
#include "Pins.h"

class Flame
{
    public:
        Flame();
        bool get_isflame(int=PIN_FLAME, bool=true);
    private:
        bool _isflame;
};

#endif