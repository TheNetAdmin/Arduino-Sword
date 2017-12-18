#include "pins.h"
#include "types.h"

const GPIO_bank gpio_bank0 = {base_addr + 0, base_addr + 1};
const GPIO_bank gpio_bank1 = {base_addr + 2, base_addr + 3};
const GPIO_bank gpio_bank2 = {base_addr + 4, base_addr + 5};
const GPIO_bank gpio_bank3 = {base_addr + 6, base_addr + 7};

struct Pin {
    GPIO_bank* bank;
    uint8_t pos;
};

struct Pin pin_map[32];

void init_pin_map() {
    // Bank 0
    pin_map[PIN_DIO0].bank = &gpio_bank0;
    pin_map[PIN_DIO0].pos = 0;
    pin_map[PIN_DIO1].bank = &gpio_bank0;
    pin_map[PIN_DIO1].pos = 1;
    pin_map[PIN_DIO2].bank = &gpio_bank0;
    pin_map[PIN_DIO2].pos = 2;
    pin_map[PIN_DIO3].bank = &gpio_bank0;
    pin_map[PIN_DIO3].pos = 3;
    pin_map[PIN_DIO4].bank = &gpio_bank0;
    pin_map[PIN_DIO4].pos = 4;
    pin_map[PIN_DIO5].bank = &gpio_bank0;
    pin_map[PIN_DIO5].pos = 5;
    pin_map[PIN_DIO6].bank = &gpio_bank0;
    pin_map[PIN_DIO6].pos = 6;
    pin_map[PIN_DIO7].bank = &gpio_bank0;
    pin_map[PIN_DIO7].pos = 7;
    // Bank 1
    pin_map[PIN_DIO8].bank = &gpio_bank1;
    pin_map[PIN_DIO8].pos = 0;
    pin_map[PIN_DIO9].bank = &gpio_bank1;
    pin_map[PIN_DIO9].pos = 1;
    pin_map[PIN_DIO10].bank = &gpio_bank1;
    pin_map[PIN_DIO10].pos = 2;
    pin_map[PIN_DIO11].bank = &gpio_bank1;
    pin_map[PIN_DIO11].pos = 3;
    pin_map[PIN_DIO12].bank = &gpio_bank1;
    pin_map[PIN_DIO12].pos = 4;
    pin_map[PIN_DIO13].bank = &gpio_bank1;
    pin_map[PIN_DIO13].pos = 5;
    // Bank 2
    pin_map[PIN_AIO0].bank = &gpio_bank2;
    pin_map[PIN_AIO0].pos = 0;
    pin_map[PIN_AIO1].bank = &gpio_bank2;
    pin_map[PIN_AIO1].pos = 1;
    pin_map[PIN_AIO2].bank = &gpio_bank2;
    pin_map[PIN_AIO2].pos = 2;
    pin_map[PIN_AIO3].bank = &gpio_bank2;
    pin_map[PIN_AIO3].pos = 3;
    pin_map[PIN_AIO4].bank = &gpio_bank2;
    pin_map[PIN_AIO4].pos = 4;
    pin_map[PIN_AIO5].bank = &gpio_bank2;
    pin_map[PIN_AIO5].pos = 5;
    pin_map[PIN_ADC_SDA].bank = &gpio_bank2;
    pin_map[PIN_ADC_SDA].pos = 6;
    pin_map[PIN_ADC_SCL].bank = &gpio_bank2;
    pin_map[PIN_ADC_SCL].pos = 7;
    // Bank3
    pin_map[PIN_PMOD0].bank = &gpio_bank3;
    pin_map[PIN_PMOD0].pos = 0;
    pin_map[PIN_PMOD1].bank = &gpio_bank3;
    pin_map[PIN_PMOD1].pos = 1;
    pin_map[PIN_PMOD2].bank = &gpio_bank3;
    pin_map[PIN_PMOD2].pos = 2;
    pin_map[PIN_PMOD3].bank = &gpio_bank3;
    pin_map[PIN_PMOD3].pos = 3;
    pin_map[PIN_PMOD4].bank = &gpio_bank3;
    pin_map[PIN_PMOD4].pos = 4;
    pin_map[PIN_PMOD5].bank = &gpio_bank3;
    pin_map[PIN_PMOD5].pos = 5;
    pin_map[PIN_PMOD6].bank = &gpio_bank3;
    pin_map[PIN_PMOD6].pos = 6;
    pin_map[PIN_PMOD7].bank = &gpio_bank3;
    pin_map[PIN_PMOD7].pos = 7;
}

void init_pin_uno() {
    set_pin_func(RX, UNO);
    set_pin_mode(RX, Input);
    set_pin_func(TX, UNO);
    set_pin_mode(TX, OutputOpenDrain);
    set_pin_func(PIN_DIO3, UNO);
    set_pin_mode(PIN_DIO3, OutputPushPull);
    set_pin_func(PIN_DIO5, UNO);
    set_pin_mode(PIN_DIO5, OutputPushPull);
    set_pin_func(PIN_DIO6, UNO);
    set_pin_mode(PIN_DIO6, OutputPushPull);
    set_pin_func(PIN_DIO9, UNO);
    set_pin_mode(PIN_DIO9, OutputPushPull);
    set_pin_func(PIN_DIO10, UNO);
    set_pin_mode(PIN_DIO10, OutputPushPull);
    set_pin_func(PIN_DIO11, UNO);
    set_pin_mode(PIN_DIO11, OutputPushPull);
}



void set_pin_func(uint8_t pin, enum PinFunc func) {
    uint32_t shift = pin_map[pin].pos * 2 + 16;  // high 16 bit
    uint32_t* reg = pin_map[pin].bank->reg1_addr;
    // unset previous func
    uint32_t pin_umask = (3 << shift) ^ max_uint32_t;
    *reg &= pin_umask;
    // set new func
    uint32_t pin_func = ((uint32_t)func) << shift;
    *reg |= pin_func;
}



void set_pin_mode(uint8_t pin, enum PinMode mode) {
    uint32_t shift = pin_map[pin].pos * 2;  // low 16 bit
    uint32_t* reg = pin_map[pin].bank->reg1_addr;
    // unset previous func
    uint32_t pin_umask = (3 << shift) ^ max_uint32_t;
    *reg &= pin_umask;
    // set new func
    uint32_t pin_func = ((uint32_t)mode) << shift;
    *reg |= pin_func;
}

uint8_t read_pin_digital_input(uint8_t pin){
    uint8_t shift = pin_map[pin].pos;
    uint32_t* reg = pin_map[pin].bank->reg0_addr;
    uint32_t value = *reg;
    value >>= shift;
    return value & 1;
}

void set_pin_digital_level(uint8_t pin, uint8_t level) {
    uint8_t shift = pin_map[pin].pos;
    uint32_t* reg = pin_map[pin].bank->reg0_addr;
    if(level == 0) {
        // LOW level, high 8 bit
        shift += 24;
    } else if (level == 1) {
        // HIGH level, 16 ~ 23 bit
        shift += 16;
    } else {
        return ;
    }
    *reg |= 1 << shift;
}