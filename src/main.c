#include <Arduino.h>
#include <vga.h>
#include <main.h>
#include "arduino/cores/pins.h"
#include "arduino/libraries/HDC1080/Melody_HDC1080.h"

void test_led() {
    kernel_clear_screen(31);
    int wait = 1000;
    int round = 0;
    while (1) {
        kernel_printf("Round: %d\n", round++);
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

void test_hdc1080() {
    begin_hdc1080();
    uint16_t result = 0xffff;
    while (1) {
        // manufacturer id
        result = getManufacturerID_hdc1080();
        kernel_printf("MID: %d\n", result);
        // device id
        result = getDeviceID_hdc1080();
        kernel_printf("DID: %d\n", result);
        // temperature
        result = getTemperature_hdc1080();
        kernel_printf("Temperature: %d\n", result);
        // humidity
        result = getHumidity_hdc1080();
        kernel_printf("Humidity: %d\n", result);
        delay(1000);
    }
}

void main() {
    test_hdc1080();
}