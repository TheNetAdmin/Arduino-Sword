#include "adxl345.h"
#include <Arduino.h>
#include "../Wire/Wire.h"

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