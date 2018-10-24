#ifndef _SWORD_PINS_H_
#define _SWORD_PINS_H_

#include <inttypes.h>

typedef struct GPIO_bank_ {
    uint32_t* reg0_addr;
    uint32_t* reg1_addr;
} GPIO_bank;

// Initial pins
void init_pin_map();
void init_pin_uno();
void init_pin_gpio();
void init_pin_adc();
void print_bank_info(uint8_t);

// Setter

typedef enum { GPIO = 0, UNO = 1, Func2 = 2, SWORD = 3 } PinFunc;
void set_pin_func(uint8_t pin, PinFunc func);

typedef enum { Input = 0, OutputOpenDrain = 1, OutputOpenSource = 2, OutputPushPull = 3 } PinMode;
void set_pin_mode(uint8_t pin, PinMode mode);

uint8_t read_pin_digital_input(uint8_t pin);
void set_pin_digital_level(uint8_t pin, uint8_t level);

#endif  // ! _SWORD_PINS_H_