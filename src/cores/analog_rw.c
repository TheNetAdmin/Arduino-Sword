#include "Arduino.h"
#include "pins.h"

void analogReference(uint8_t mode) {
    // TODO
}

int analogRead(uint8_t pin) {
    // TODO
}

void set_pwm(uint8_t pwm_channel, uint32_t value) {
    uint32_t * pwm_addr = (uint32_t*)0xbfc09228;
    if(pwm_channel > 5){
        return ;
    }
    pwm_addr += pwm_channel;
    *pwm_addr = value;
}

void analogWrite(uint8_t pin, uint8_t value) {
    static uint32_t max_value = 0xffff;
    uint32_t input_value = ((uint32_t)value << 16) | (uint32_t)value;
    uint32_t high = input_value << 16;
    uint32_t low = max_value - input_value;
    uint8_t pwm_channel = 6;
    switch(pin){
        case PIN_DIO3: pwm_channel = 0;
        case PIN_DIO5: pwm_channel = 1;
        case PIN_DIO6: pwm_channel = 2;
        case PIN_DIO9: pwm_channel = 3;
        case PIN_DIO10: pwm_channel = 4;
        case PIN_DIO11: pwm_channel = 5;
        default: pwm_channel = 6;
    }
    set_pwm(pwm_channel, high|low);
}
