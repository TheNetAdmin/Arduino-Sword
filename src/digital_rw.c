#include "Arduino.h"

void pinMode(uint8_t pin, enum DigitalPinMode mode) {
    switch (mode) {
        case INPUT:
            set_pin_mode(pin, Input);
        case OUTPUT:
            set_pin_mode(pin, OutputPushPull);
        default:
            return;
    }
}

void digitalWrite(uint8_t pin, enum DigitalPinLevel level) {
    switch(level){
        case LOW:
            set_pin_digital_level(pin, 0);
        case HIGH:
            set_pin_digital_level(pin, 1);
        default:
            return;
    }
}

int digitalRead(uint8_t pin) {
    return (int)read_pin_digital_input(pin);
}