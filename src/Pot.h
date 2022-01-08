#ifndef POT_H
#define POT_H

#include <Arduino.h>
#include "Pins.h"

class Pot
{
    public:
        Pot();
        int get_pot(int=POT, bool=true);
    private:
        float _potval;
};

#endif