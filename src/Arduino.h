#ifndef _SWRODD_ARDUINO_H_
#define _SWRODD_ARDUINO_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initial arduino
void init_arduino();

// Digital read & write
enum DigitalPinMode{
    INPUT = 0,
    OUTPUT = 1
};
void pinMode(uint8_t pin, enum DigitalPinMode mode);
enum DigitalPinLevel{
    LOW = 0,
    HIGH = 1
};
void digitalWrite(uint8_t pin, enum DigitalPinLevel level);
int digitalRead(uint8_t pin);

// Analog read & write
int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, uint8_t value);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // ! _SWRODD_ARDUINO_H_
