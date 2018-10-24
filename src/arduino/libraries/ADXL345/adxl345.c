#include "adxl345.h"
#include <Arduino.h>
#include <vga.h>
#include "../Wire/Wire.h"
#include "../i2c_hardware/i2c_hardware.h"


uint8_t data_ready = false;

void _write_adxl345(uint8_t regAddress, uint8_t *pdata, uint8_t length) {
    begin_transmission_wire(ADXLADDRESS);
    write_wire_byte(regAddress);
    write_wire(pdata, length);
    end_transmission_wire(true);
}

void _write_byte_adxl345(uint8_t regAddress, uint8_t data) { _write_adxl345(regAddress, &data, 1); }

uint8_t _read_adxl345(uint8_t regAddress, uint8_t *pdata, uint8_t length) {
    uint8_t read_length = 0;
    begin_transmission_wire(ADXLADDRESS);
    write_wire_byte(regAddress);
    end_transmission_wire(true);

    request_from_wire(ADXLADDRESS, length, true);
    while (available_wire()) {
        pdata[read_length++] = read_wire();
    }
    return read_length;
}

uint8_t _read_byte_adxl345(uint8_t regAddress) {
    uint8_t data = 0;
    begin_transmission_wire(ADXLADDRESS);
    write_wire_byte(regAddress);
    end_transmission_wire(true);

    request_from_wire(ADXLADDRESS, 1, true);
    if (available_wire() >= 1) {
        data = read_wire();
    }
    return data;
}

void _start_sequence() {
    _write_byte_adxl345(ADXLREG_DATAFORMAT, ADXLVALUE_DATAFORMAT);
    _write_byte_adxl345(ADXLREG_BWRATE, ADXLVALUE_BWRATE);
    _write_byte_adxl345(ADXLREG_POWERCTL, ADXLVALUE_POWERCTL);
    _write_byte_adxl345(ADXLREG_INTENABLE, ADXLVALUE_INTENABLE);
    _write_byte_adxl345(ADXLREG_OFSX, ADXLVALUE_OFSX);
    _write_byte_adxl345(ADXLREG_OFSY, ADXLVALUE_OFSY);
    _write_byte_adxl345(ADXLREG_OFSZ, ADXLVALUE_OFSZ);
}

void begin_adxl345() {
    begin_wire();
    _start_sequence();
    delay(200);
}

bool available() {
    if (data_ready) {
        data_ready = false;
        return true;
    } else {
        return false;
    }
}

int16_t get_x_accelerate_adxl345() {
    uint8_t data[2] = {0};
    _read_adxl345(ADXLREG_DATAX0, data, 2);
    return ((uint16_t)(data[1] << 8) | data[0]);
}

int16_t get_y_accelerate_adxl345() {
    uint8_t data[2] = {0};
    _read_adxl345(ADXLREG_DATAY0, data, 2);
    return ((uint16_t)(data[1] << 8) | data[0]);
}

int16_t get_z_accelerate_adxl345() {
    uint8_t data[2] = {0};
    _read_adxl345(ADXLREG_DATAZ0, data, 2);
    return ((uint16_t)(data[1] << 8) | data[0]);
}

Accelerate get_accelerate_adxl345() {
    uint8_t data[6] = {0};
    _read_adxl345(ADXLREG_DATAZ0, data, 6);

    Accelerate accelerate = {0};
    accelerate.X = (uint16_t)(data[1] << 8) | data[0];
    accelerate.Y = (uint16_t)(data[3] << 8) | data[2];
    accelerate.Z = (uint16_t)(data[5] << 8) | data[4];

    return accelerate;
}

uint8_t get_device_id_adxl345() { return _read_byte_adxl345(ADXLREG_DEVICEID); }

void hw_begin_adxl345(){
    write_i2c_h(ADXLADDRESS, ADXLREG_DATAFORMAT, 1, ADXLVALUE_DATAFORMAT);
    write_i2c_h(ADXLADDRESS, ADXLREG_BWRATE, 1, ADXLVALUE_BWRATE);
    write_i2c_h(ADXLADDRESS, ADXLREG_POWERCTL, 1, ADXLVALUE_POWERCTL);
    write_i2c_h(ADXLADDRESS, ADXLREG_INTENABLE, 1, ADXLVALUE_INTENABLE);
    write_i2c_h(ADXLADDRESS, ADXLREG_OFSX, 1, ADXLVALUE_OFSX);
    write_i2c_h(ADXLADDRESS, ADXLREG_OFSY, 1, ADXLVALUE_OFSY);
    write_i2c_h(ADXLADDRESS, ADXLREG_OFSZ, 1, ADXLVALUE_OFSZ);
}

int16_t hw_get_x_accelerate_adxl345(){
    int8_t data[2];
    data[0] = (int16_t)read_i2c_h(ADXLADDRESS, 0x32,1);
    data[1] = (int16_t)read_i2c_h(ADXLADDRESS, 0x33,1);
    return (int16_t)((uint16_t)data[1] << 8 | data[0]);
}


int16_t hw_get_y_accelerate_adxl345(){
    int8_t data[2];
    data[0] = (int16_t)read_i2c_h(ADXLADDRESS, 0x34,1);
    data[1] = (int16_t)read_i2c_h(ADXLADDRESS, 0x35,1);
    return (int16_t)((uint16_t)data[1] << 8 | data[0]);
}


int16_t hw_get_z_accelerate_adxl345(){
    int8_t data[2];
    data[0] = (int16_t)read_i2c_h(ADXLADDRESS, 0x36,1);
    data[1] = (int16_t)read_i2c_h(ADXLADDRESS, 0x37,1);
    return (int16_t)((uint16_t)data[1] << 8 | data[0]);
}


/*read from adxl345 by hardware i2c*/
void hw_i2c_test_adxl345(){
    init_i2c_h();
    set_i2c_restart_hold_cycle(10);//10us
    delay(200);
    int16_t result = 0;
    while(1) {
        hw_begin_adxl345();
        result = hw_get_x_accelerate_adxl345();
        printf("X: %d, ", result);
        result = hw_get_y_accelerate_adxl345();
        printf("Y: %d, ", result);
        result = hw_get_z_accelerate_adxl345();
        printf("Z: %d\n", result);
        delay(2000);
    }
}

