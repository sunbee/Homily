/*
Copyright of Sanjay Bhatikar
*/
#include "Float.h"

Float::Float(int pin_no) {
    this->_pin = pin_no;
}

bool Float::get_isflood(bool live) {
    if (live) {
        this->_isflood = digitalRead(this->_pin);
    }
    return this->_pin;

}
