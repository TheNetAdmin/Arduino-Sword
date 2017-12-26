#include "Melody_HDC1080.h"
#include <Arduino.h>
#include <inttypes.h>
#include <vga.h>
#include "../I2C/i2c.h"
#include "../Wire/Wire.h"

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
