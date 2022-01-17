/*
Copyright of Sanjay Bhatikar
*/
#ifndef GAS_H
#define GAS_H

#include <Arduino.h>
#include "Pins.h"

class Gas 
{
    public:
        Gas();
        bool get_isgas(int PINB_GAS, bool=true);
        int get_gasval(int PINA_GAS, bool=true);
    private:
        bool _isgas;
        int _gasval;
};

#endif