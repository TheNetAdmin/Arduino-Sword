#ifndef _ARDUINO_SIMULATE_H_
#define _ARDUINO_SIMULATE_H_

#include <Arduino.h>
#include <main.h>
#include <vga.h>
#include "arduino/cores/pins.h"

void setup();
void loop();

class HDC1080 {
   public:
    void init();
    void setI2CHoldTime(uint32_t us);
    void begin();
    uint16_t getTempterature();
    uint16_t getHumility();
};

#endif /* _ARDUINO_SIMULATE_H_ */