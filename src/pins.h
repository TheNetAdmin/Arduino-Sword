#ifndef _SWORD_PINS_H_
#define _SWORD_PINS_H_

#include "types.h"

static const uint32_t* const base_addr = (uint32_t*)0xbfc09200;

typedef struct GPIO_bank_ {
    uint32_t* reg0_addr;
    uint32_t* reg1_addr;
} GPIO_bank;

#define PIN_DIO0 (0)
#define PIN_DIO1 (1)
#define PIN_DIO2 (2)
#define PIN_DIO3 (3)
#define PIN_DIO4 (4)
#define PIN_DIO5 (5)
#define PIN_DIO6 (6)
#define PIN_DIO7 (7)
#define PIN_DIO8 (8)
#define PIN_DIO9 (8)
#define PIN_DIO10 (10)
#define PIN_DIO11 (11)
#define PIN_DIO12 (12)
#define PIN_DIO13 (13)
#define PIN_AIO0 (14)
#define PIN_AIO1 (15)
#define PIN_AIO2 (16)
#define PIN_AIO3 (17)
#define PIN_AIO4 (18)
#define PIN_AIO5 (19)
#define PIN_ADC_SDA (20)
#define PIN_ADC_SCL (21)
#define PIN_PMOD0 (22)
#define PIN_PMOD1 (23)
#define PIN_PMOD2 (24)
#define PIN_PMOD3 (25)
#define PIN_PMOD4 (26)
#define PIN_PMOD5 (27)
#define PIN_PMOD6 (28)
#define PIN_PMOD7 (29)

static const uint8_t TX = PIN_DIO0;
static const uint8_t RX = PIN_DIO1;
static const uint8_t A0 = PIN_AIO0;
static const uint8_t A1 = PIN_AIO1;
static const uint8_t A2 = PIN_AIO2;
static const uint8_t A3 = PIN_AIO3;
static const uint8_t A4 = PIN_AIO4;
static const uint8_t A5 = PIN_AIO5;

// Initial pins
void init_pin_map();
void init_pin_uno();

// Setter

enum PinFunc { GPIO = 0, UNO = 1, Func2 = 2, SWORD = 3 };
void set_pin_func(uint8_t pin, enum PinFunc func);

enum PinMode { Input = 0, OutputOpenDrain = 1, OutputOpenSource = 2, OutputPushPull = 3 };
void set_pin_mode(uint8_t pin, enum PinMode mode);

uint8_t read_pin_digital_input(uint8_t pin);
void set_pin_digital_level(uint8_t pin, uint8_t level);

#endif  // ! _SWORD_PINS_H_