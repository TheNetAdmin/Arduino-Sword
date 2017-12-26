#include <Arduino.h>
#include <vga.h>
#include "../libraries/I2C/i2c.h"
#include "pins.h"

void analogReference(uint8_t mode) {
    // TODO
}

uint16_t analogRead(uint8_t pin) {
    uint8_t cs = 0;
    switch (pin) {
        case PIN_AIO0:
            cs = 0;
            break;
        case PIN_AIO1:
            cs = 1;
            break;
        case PIN_AIO2:
            cs = 2;
            break;
        case PIN_AIO3:
            cs = 3;
            break;
        case PIN_AIO4:
            cs = 4;
            break;
        case PIN_AIO5:
            cs = 5;
            break;
        default:
            return 0;
    }

    // read original pins
    uint8_t origin_sda, origin_scl;
    origin_sda = get_sda_i2c();
    origin_scl = get_scl_i2c();

    // use adc chip pins
    set_pins_i2c(PIN_ADC_SDA, PIN_ADC_SCL);

    // max11609 addr 0110011
    uint8_t addr = 0x33;
    uint8_t conf = (cs << 1) | 0x61;
    write_i2c(addr, &conf, 1, true);

    // read from
    uint8_t result[2];
    read_i2c(addr, result, 2, true);

    // reset original pins
    set_pins_i2c(origin_sda, origin_scl);

    return (uint16_t)((result[0] << 8) | (result[1])) & (uint16_t)0x03ff;
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
