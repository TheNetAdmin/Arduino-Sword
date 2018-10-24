#include <Arduino.h>
#include <vga.h>
#include <main.h>
#include "arduino/cores/pins.h"
#include "arduino/libraries/HDC1080/Melody_HDC1080.h"
#include "arduino/libraries/LMT84/lmt84.h"
#include "arduino/libraries/ADXL345/adxl345.h"
#include "arduino/libraries/I2C_Hardware/i2c_hardware.h"
#include "ArduinoSimulate.h"

void HDC1080::begin(){
    hw_begin_hdc1080();
}

void HDC1080::init(){
    init_i2c_h();
}

void HDC1080::setI2CHoldTime(uint32_t us){
    set_i2c_restart_hold_time(us);
}

uint16_t HDC1080::getTempterature(){
    return hw_getTemperature_hdc1080();
}

uint16_t HDC1080::getHumility(){
    return hw_getHumidity_hdc1080();
}

HDC1080 hdc;

void setup(){
    set_pin_func(PIN_AIO4, UNO);
    set_pin_mode(PIN_AIO4, OutputOpenDrain);
    set_pin_func(PIN_AIO5, UNO);
    set_pin_mode(PIN_AIO5, OutputOpenDrain);

    hdc.init();
    hdc.setI2CHoldTime(40000);//40ms
    hdc.begin();
}

void loop(){
    uint32_t temperature = hdc.getTempterature();
    uint32_t humidity = hdc.getHumility();
    printf("temperature: %d, humidity: %d\n", temperature, humidity);
    delay(1000);
}