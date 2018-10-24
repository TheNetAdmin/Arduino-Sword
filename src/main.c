#include <Arduino.h>
#include <vga.h>
#include <main.h>
#include "arduino/cores/pins.h"
#include "arduino/libraries/HDC1080/Melody_HDC1080.h"
#include "arduino/libraries/LMT84/lmt84.h"
#include "arduino/libraries/ADXL345/adxl345.h"
#include "arduino/libraries/I2C_Hardware/i2c_hardware.h"
#include "ArduinoSimulate.h"

void test_led() {
    clear_screen(31);
    int wait = 1000;
    int round = 0;
    init_pin_uno();
    while (1) {
        printf("Round: %d\n", round++);
        analogWrite(PIN_DIO3, 0);
        delay(wait);
        analogWrite(PIN_DIO3, 63);
        delay(wait);
        analogWrite(PIN_DIO3, 127);
        delay(wait);
        analogWrite(PIN_DIO3, 255);
        delay(wait);
    }
}



int main() {
    // global_init_i2c_h();
    // hw_i2c_test_hdc1080();
    //hw_i2c_test_adxl345();
    //test_led(); 
    //test_lmt84(); // basing on hardware i2c
    setup();
    for(;;){
        loop();
    }
}

