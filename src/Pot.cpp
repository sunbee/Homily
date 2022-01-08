/*
Copyright of Sanjay Bhatikar
*/
#include "Pot.h"

Pot::Pot() {

}

int Pot::get_pot(int pin, bool live) {
    if (live) {
        this->_potval = analogRead(pin);
    }
    return this->_potval;
}