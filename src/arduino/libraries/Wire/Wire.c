#include <Arduino.h>
#include <vga.h>
#include <inttypes.h>
#include "Wire.h"
#include "../I2C/i2c.h"

static uint8_t rxBuffer[BUFFER_LENGTH];
static uint8_t rxBufferIndex = 0;
static uint8_t rxBufferLength = 0;

static uint8_t txAddress = 0;
static uint8_t txBuffer[BUFFER_LENGTH];
static uint8_t txBufferIndex = 0;
static uint8_t txBufferLength = 0;

static uint8_t transmitting = 0;

void begin_wire() {
    rxBufferIndex = 0;
    rxBufferLength = 0;

    txBufferIndex = 0;
    txBufferLength = 0;

    init_i2c();
}

void end_wire() { stop_send_i2c(); }

void begin_transmission_wire(uint8_t address) {
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
}

uint8_t end_transmission_wire(uint8_t sendStop) {
    // transmit buffer (blocking)
    uint8_t ret = write_i2c(txAddress, txBuffer, txBufferLength, sendStop);
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
    // indicate that we are done transmitting
    transmitting = 0;
    return ret;
}

uint8_t request_from_wire(uint8_t addr, uint8_t length, uint8_t send_stop) {
    if (length > BUFFER_LENGTH) {
        length = BUFFER_LENGTH;
    }
    uint8_t result = read_i2c(addr, rxBuffer, length, send_stop);

    rxBufferIndex = 0;
    rxBufferLength = length;

    return length;
}

size_t write_wire_byte(uint8_t data) {
    if (transmitting) {
        // in master transmitter mode
        // don't bother if buffer is full
        if (txBufferLength >= BUFFER_LENGTH) {
            return 0;
        }
        // put byte in tx buffer
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        // update amount in buffer
        txBufferLength = txBufferIndex;
    }
    return 1;
}

size_t write_wire(const uint8_t *data, size_t length) {
    uint8_t tmp = 0;
    if (transmitting) {
        // in master transmitter mode
        for (size_t i = 0; i < length; ++i) {
            write_wire_byte(data[i]);
        }
    }
    return length;
}

int available_wire() { return rxBufferLength - rxBufferIndex; }

int read_wire() {
    int value = -1;

    // get each successive byte on each call
    if (rxBufferIndex < rxBufferLength) {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }

    return value;
}
