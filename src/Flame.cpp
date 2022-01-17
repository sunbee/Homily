/*
Copyright of Sanjay Bhatikar
*/
#include "Flame.h"

Flame::Flame() {

}

bool Flame::get_isflame(int pin, bool live) {
    if (live) {
        this->_isflame = digitalRead(pin);
    }
    return this->_isflame;
}