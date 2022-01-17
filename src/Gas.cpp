/*
Copyright of Sanjay Bhatikar
*/
#include "Gas.h"

Gas::Gas() {

}

bool Gas::get_isgas(int pin, bool live) {
    if (live) {
        this->_isgas = digitalRead(pin);
    }
    return this->_isgas;
}

int Gas::get_gasval(int pin, bool live) {
    if (live) {
        this->_gasval = analogRead(pin);
    }
    return this->_gasval;
}