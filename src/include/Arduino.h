#ifndef _SWRODD_ARDUINO_H_
#define _SWRODD_ARDUINO_H_

#include <inttypes.h>

#define NULL 0

// Initial arduino
void init_arduino();

// Digital read & write
typedef enum { INPUT = 0, OUTPUT = 1 } DigitalPinMode;
void pinMode(uint8_t pin, DigitalPinMode mode);
typedef enum { LOW = 0, HIGH = 1 } DigitalPinLevel;
void digitalWrite(uint8_t pin, DigitalPinLevel level);
int digitalRead(uint8_t pin);

// Analog read & write
uint16_t analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, uint8_t value);

// Delay
void delay(uint32_t ms);
void wait_usec(uint32_t usec);

// Pins

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

#endif  // ! _SWRODD_ARDUINO_H_
