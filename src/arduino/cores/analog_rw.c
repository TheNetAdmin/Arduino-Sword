#include <Arduino.h>
#include <vga.h>
#include "pins.h"

void analogReference(uint8_t mode) {
    // TODO
}

int analogRead(uint8_t pin) {
    // TODO
}

void set_pwm(uint8_t pwm_channel, uint32_t value) {
    uint32_t* pwm_addr = (uint32_t*)0xbfc09228;
    if (pwm_channel > 5) {
        return;
    }
    pwm_addr += pwm_channel;
    *pwm_addr = value;
}

void analogWrite(uint8_t pin, uint8_t value) {
    static uint32_t max_value = 0xff;
    uint32_t high = (((uint32_t)value) * 257) << 16;
    uint32_t low = (uint32_t)(max_value - value) * 257;
    uint8_t pwm_channel = 6;
    switch (pin) {
        case PIN_DIO3:
            pwm_channel = 0;
            break;
        case PIN_DIO5:
            pwm_channel = 1;
            break;
        case PIN_DIO6:
            pwm_channel = 2;
            break;
        case PIN_DIO9:
            pwm_channel = 3;
            break;
        case PIN_DIO10:
            pwm_channel = 4;
            break;
        case PIN_DIO11:
            pwm_channel = 5;
            break;
        default:
            pwm_channel = 6;
            break;
    }
    set_pwm(pwm_channel, high | low);
}
