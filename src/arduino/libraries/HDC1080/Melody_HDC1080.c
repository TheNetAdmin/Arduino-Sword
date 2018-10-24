#include "Melody_HDC1080.h"
#include <Arduino.h>
#include <inttypes.h>
#include <vga.h>
#include "../I2C/i2c.h"
#include "../Wire/Wire.h"
#include "../i2c_hardware/i2c_hardware.h"



uint16_t _read_hdc1080(uint8_t reg_addr) {
    set_pins_i2c(PIN_AIO4, PIN_AIO5);
    uint16_t data = 0;
    begin_transmission_wire(HDC_ADDRESS);
    write_wire_byte(reg_addr);
    end_transmission_wire(true);

    delay(40);

    request_from_wire(HDC_ADDRESS, 2, true);
    if (available_wire() >= 2) {
        data = read_wire() << 8;
        data |= read_wire();
    } else {
        printf("_read_hdc1080: no enough data\n");
    }
    return data;
}

void _write_hdc1080(uint8_t reg_addr, uint8_t *data, uint8_t length) {
    set_pins_i2c(PIN_AIO4, PIN_AIO5);
    begin_transmission_wire(HDC_ADDRESS);
    write_wire_byte(reg_addr);
    write_wire(data, length);
    end_transmission_wire(true);
}

void begin_hdc1080() {
    begin_wire();

    uint8_t confdata[2] = {0};
    confdata[0] = (HDC_CONFIGURATION >> 8) & 0xFF;
    confdata[1] = HDC_CONFIGURATION & 0xFF;

    _write_hdc1080(HDCREG_CONFIGURATION, confdata, 2);
}

uint16_t getTemperature_hdc1080() {
    uint16_t temperature = 0;
    temperature = _read_hdc1080(HDCREG_TEMPERATURE);

    return temperature;
    // return (temperature / 65536.0 * 165.0 - 40.0);
}

uint16_t getHumidity_hdc1080() {
    uint16_t humidity = 0;
    humidity = _read_hdc1080(HDCREG_HUMIDITY);

    return humidity;
    // return (humidity / 65536.0 * 100.0);
}

uint16_t getManufacturerID_hdc1080() {
    return _read_hdc1080(HDCREG_MANUFACTURERID);
}

uint16_t getDeviceID_hdc1080() {
    return _read_hdc1080(HDCREG_DEVICEID);
}

uint16_t *getSerialID_hdc1080(uint16_t *data, uint8_t length) {
    if (length < 3) {
        return NULL;
    }
    data[0] = _read_hdc1080(HDCREG_SERIALID_FRIST);
    data[1] = _read_hdc1080(HDCREG_SERIALID_MID);
    data[2] = _read_hdc1080(HDCREG_SERIALID_LAST);

    return data;
}


void hw_begin_hdc1080(){
    uint8_t addr_dev = 0x40;
    uint8_t addr_reg_temperature = 0x00;
    uint8_t addr_reg_humidity = 0x01;
    uint8_t addr_reg_config = 0x02;
    uint8_t length = 2;
    uint32_t mode = 0x00001000;
    write_i2c_h(addr_dev, addr_reg_config, length, mode);
}


uint16_t hw_getTemperature_hdc1080(){
    uint8_t addr_dev = 0x40;
    uint8_t addr_reg_temperature = 0x00;
    uint8_t length = 2;
    return read_i2c_h(addr_dev,addr_reg_temperature,length);
}


uint16_t hw_getHumidity_hdc1080(){
    uint8_t addr_dev = 0x40;
    uint8_t addr_reg_humidity = 0x01;
    uint8_t length = 2;
    return read_i2c_h(addr_dev,addr_reg_humidity,length);
}

/*read from hdc1080 by hardware i2c*/
void hw_i2c_test_hdc1080() {
    init_i2c_h();
    set_i2c_restart_hold_time(40000);//40ms
    hw_begin_hdc1080();
    while(1){
        uint32_t temperature = hw_getTemperature_hdc1080();
        uint32_t humidity = hw_getHumidity_hdc1080();
        printf("temperature: %d, humidity: %d\n", temperature, humidity);
        delay(1000);
    }
}