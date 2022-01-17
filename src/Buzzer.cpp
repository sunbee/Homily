/*
Copyright of Sanjay Bhatikar
*/
#include "Buzzer.h"

Buzzer::Buzzer(int pin) {
    digitalWrite(pin, HIGH);
}

void sound_alarm(int pin, int duration) {
    unsigned long tic = millis();
    unsigned long toc = millis();
    digitalWrite(pin, LOW);
    while ((toc - tic) < duration) {
        toc = millis();
        // do nothing
    }
    digitalWrite(pin, HIGH);
}