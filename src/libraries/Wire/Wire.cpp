extern "C" {
#include <inttypes.h>
#include "../I2C/i2c.h"
}

#include "Wire.h"

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;

TwoWire::TwoWire()
= default;

void TwoWire::begin() {
    rxBufferIndex = 0;
    rxBufferLength = 0;

    txBufferIndex = 0;
    txBufferLength = 0;

    init_i2c();
}

void TwoWire::end() {
    stop_send_i2c();
}

void TwoWire::beginTransmission(uint8_t address) {
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
    // transmit buffer (blocking)
    uint8_t ret = write_i2c(txAddress, txBuffer, txBufferLength, sendStop);
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
    // indicate that we are done transmitting
    transmitting = 0;
    return ret;
}

uint8_t TwoWire::endTransmission() {
    return endTransmission((uint8_t) true);
}

uint8_t TwoWire::requestFrom(uint8_t addr, uint8_t quantity) {
    return requestFrom(addr, quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int addr, int quantity) {
    return requestFrom((uint8_t) addr, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(uint8_t addr, uint8_t length, uint8_t send_stop) {
    if (length > BUFFER_LENGTH) {
        length = BUFFER_LENGTH;
    }
    uint8_t result = read_i2c(addr, rxBuffer, length, send_stop);

    rxBufferIndex = 0;
    rxBufferLength = result;

    return result;
}

size_t TwoWire::write(uint8_t data) {
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

size_t TwoWire::write(const uint8_t *data, size_t length) {
    if (transmitting) {
        // in master transmitter mode
        for (size_t i = 0; i < length; ++i) {
            write(data[i]);
        }
    }
    return length;
}

int TwoWire::available() {
    return rxBufferLength - rxBufferIndex;
}

int TwoWire::read() {
    int value = -1;

    // get each successive byte on each call
    if (rxBufferIndex < rxBufferLength) {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }

    return value;
}

