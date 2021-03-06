#include <Arduino.h>
#include "pins.h"

void pinMode(uint8_t pin, DigitalPinMode mode) {
    switch (mode) {
        case INPUT:
            set_pin_mode(pin, Input);
            break;
        case OUTPUT:
            set_pin_mode(pin, OutputPushPull);
            break;
        default:
            return;
    }
}

void digitalWrite(uint8_t pin, DigitalPinLevel level) {
    switch (level) {
        case LOW:
            set_pin_digital_level(pin, 0);
            break;
        case HIGH:
            set_pin_digital_level(pin, 1);
            break;
        default:
            return;
    }
}

int digitalRead(uint8_t pin) {
    return (int)read_pin_digital_input(pin);
}