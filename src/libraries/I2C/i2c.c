#include "i2c.h"
#include "i2c-private.h"
#include <cores/Arduino.h>
#include <cores/pins.h>

// Call back funcs
static void (*on_slave_transmit)(void);

static void (*on_slave_receive)(uint8_t *, int);

// Platform dependent functions

void wait_usec(uint32_t usec) {
    // TODO
}

// Implementations

static I2CState curr_state;
static const uint32_t i2c_time = 2;
static const uint8_t i2c_read = 0x01;
static const uint8_t i2c_write = 0x00;

void stop_send_i2c() {
    pinMode(PIN_ADC_SCL, OUTPUT);
    digitalWrite(PIN_ADC_SCL, LOW);
    pinMode(PIN_ADC_SDA, OUTPUT);
    digitalWrite(PIN_ADC_SDA, LOW);
    wait_usec(i2c_time);

    pinMode(PIN_ADC_SCL, INPUT);
    while (!digitalRead(PIN_ADC_SCL)) {
    }
    wait_usec(1);

    pinMode(PIN_ADC_SDA, INPUT);
}

I2CResult i2c_start_send() {
    uint32_t i = 0;
    pinMode(PIN_ADC_SDA, INPUT);
    pinMode(PIN_ADC_SCL, INPUT);
    wait_usec(1);

    while (i < 50) {
        if (!digitalRead(PIN_ADC_SDA)) {
            return START_FAIL_BUSY;
        }
        if (!digitalRead(PIN_ADC_SCL)) {
            return START_FAIL_BUSY;
        }
        wait_usec(1);
        i++;
    }

    pinMode(PIN_ADC_SDA, OUTPUT);
    digitalWrite(PIN_ADC_SDA, LOW);
    wait_usec(i2c_time);

    return START_SUCC;
}

I2CResult i2c_send_sla(uint8_t d_addr) {
    I2CResult result_state = i2c_send_byte(d_addr);
    if (result_state == SEND_BYTE_SUCC) {
        if (d_addr & i2c_read) {
            // SLA R OK
            return SEND_SLA_R_SUCC;
        } else {
            // SLA W OK
            return SEND_SLA_W_SUCC;
        }
    } else if (result_state == SEND_BYTE_NACK) {
        if (d_addr & i2c_read) {
            // SLA R NACK
            return SEND_SLA_R_NACK;
        } else {
            // SLA W NACK
            return SEND_SLA_W_NACK;
        }
    } else if (result_state == SEND_BYTE_FAIL) {
        return BUS_ERROR;
    }
}

I2CResult i2c_send_byte(const uint8_t data) {
    uint8_t pin_state = 0;

    // send bit by bit, 8 bits total
    for (int i = 0; i < 8; ++i) {
        pinMode(PIN_ADC_SCL, OUTPUT);
        digitalWrite(PIN_ADC_SCL, LOW);
        if (data & (0x01 << (7 - i))) {
            // send high
            if (!digitalRead(PIN_ADC_SDA)) {
                pinMode(PIN_ADC_SDA, INPUT);
            }
            pin_state = 1;
        } else {
            // send low
            if (digitalRead(PIN_ADC_SDA)) {
                pinMode(PIN_ADC_SDA, OUTPUT);
                digitalWrite(PIN_ADC_SDA, LOW);
            }
            pin_state = 0;
        }
        wait_usec(i2c_time);

        // SCL H transmit data
        pinMode(PIN_ADC_SCL, INPUT);
        while (!digitalRead(PIN_ADC_SCL)) {
        }
        if (pin_state != digitalRead(PIN_ADC_SDA)) {
            return SEND_BYTE_FAIL;
        }
        wait_usec(i2c_time);
    }

    // check ack/nack
    // scl low
    pinMode(PIN_ADC_SCL, OUTPUT);
    digitalWrite(PIN_ADC_SCL, LOW);
    pinMode(PIN_ADC_SDA, INPUT);
    wait_usec(i2c_time);
    // scl high
    pinMode(PIN_ADC_SCL, INPUT);
    while (!digitalRead(PIN_ADC_SCL)) {
    }
    if (!digitalRead(PIN_ADC_SDA)) {
        wait_usec(i2c_time);
        return SEND_BYTE_SUCC;
    } else {
        wait_usec(i2c_time);
        return SEND_BYTE_NACK;
    }
}

I2CResult i2c_send_data(const uint8_t *buffer, uint32_t length) {
    for (int i = 0; i < length; ++i) {
        I2CResult result_state = i2c_send_byte(*buffer++);
        if (result_state == SEND_BYTE_NACK) {
            return SEND_DATA_NACK;
        } else if (result_state == SEND_BYTE_FAIL) {
            return SEND_DATA_FAIL;
        }
    }
    return SEND_DATA_SUCC;
}

void init_i2c() {
    if (curr_state != I2C_NO_INIT) {
        return;
    }
    pinMode(PIN_ADC_SCL, INPUT);
    pinMode(PIN_ADC_SDA, INPUT);
    curr_state = I2C_OK;
}

void bus_clear() {
    pinMode(PIN_ADC_SDA, INPUT);
    for (int i = 0; i < 9; ++i) {
        pinMode(PIN_ADC_SCL, OUTPUT);
        digitalWrite(PIN_ADC_SCL, LOW);
        wait_usec(i2c_time);
        pinMode(PIN_ADC_SCL, INPUT);
        while (!digitalRead(PIN_ADC_SCL)) {
        }
        wait_usec(i2c_time);
    }
    stop_send_i2c();
}

I2CState i2c_send_error(I2CResult state) {
    if (state == DEFAULT) {
    } else if (state == START_FAIL_BUSY) {
        curr_state = I2C_BUSY;
    }
    stop_send_i2c();
    return curr_state;
}

I2CResult i2c_restart_send() {
    // SCL LOW send start condition
    pinMode(PIN_ADC_SCL, OUTPUT);
    digitalWrite(PIN_ADC_SCL, LOW);
    pinMode(PIN_ADC_SDA, INPUT);
    wait_usec(i2c_time);

    // set start condition
    pinMode(PIN_ADC_SCL, INPUT);
    while (!digitalRead(PIN_ADC_SCL)) {
    }
    pinMode(PIN_ADC_SDA, OUTPUT);
    digitalWrite(PIN_ADC_SDA, LOW);
    wait_usec(i2c_time);

    return RESTART_END;
}

I2CResult i2c_receive_data(uint8_t *buffer, uint32_t length) {
    for (int i = 0; i < length; ++i) {
        *buffer = 0x00;
        pinMode(PIN_ADC_SCL, OUTPUT);
        digitalWrite(PIN_ADC_SCL, LOW);
        pinMode(PIN_ADC_SDA, INPUT);
        wait_usec(i2c_time);

        // read bit by bit
        for (int j = 0; j < 8; ++j) {
            // SCL HIGH
            pinMode(PIN_ADC_SCL, INPUT);
            while (!digitalRead(PIN_ADC_SCL)) {
            }
            *buffer |= digitalRead(PIN_ADC_SDA) << (7 - j);
            wait_usec(i2c_time);

            // SCL LOW
            pinMode(PIN_ADC_SCL, OUTPUT);
            digitalWrite(PIN_ADC_SCL, LOW);
            wait_usec(i2c_time);
        }

        // next byte
        ++buffer;

        if (i == length - 1) {
            // SCL HIGH
            wait_usec(i2c_time);
            pinMode(PIN_ADC_SCL, INPUT);
            while (!digitalRead(PIN_ADC_SCL)) {
            };
            wait_usec(i2c_time);
            break;
        } else {
            pinMode(PIN_ADC_SDA, OUTPUT);
            digitalWrite(PIN_ADC_SDA, LOW);
            wait_usec(i2c_time);
            // SCL HIGH
            pinMode(PIN_ADC_SCL, INPUT);
            while (!digitalRead(PIN_ADC_SCL)) {
            }
            wait_usec(i2c_time);
        }
    }
    return RECV_DATA_END;
}

I2CState write_i2c(uint8_t dev_addr, const uint8_t *data, uint8_t length, uint8_t send_stop) {
    I2CResult result_state;
    if (curr_state == I2C_NO_INIT) {
        init_i2c();
    }

    result_state = i2c_start_send();
    if (result_state != START_SUCC) {
        return i2c_send_error(result_state);
    }

    result_state = i2c_send_sla(dev_addr + i2c_write);
    if (result_state != SEND_SLA_W_SUCC) {
        i2c_send_error(result_state);
    }

    result_state = i2c_send_data(data, length);
    if (result_state != SEND_DATA_SUCC) {
        i2c_send_error(result_state);
    }

    if (0 == send_stop) {
        // stop_send == false
        stop_send_i2c();
    }
    curr_state = I2C_OK;
    return curr_state;
}

I2CState read_i2c(uint8_t dev_addr, uint8_t *buffer, uint32_t length, uint8_t stop_send) {
    I2CResult result_state;

    if (curr_state == I2C_NO_INIT) {
        init_i2c();
    }

    result_state = i2c_start_send();
    if (result_state != START_SUCC) {
        return i2c_send_error(result_state);
    }

    result_state = i2c_send_sla(dev_addr + i2c_write);
    if (result_state != SEND_SLA_W_SUCC) {
        return i2c_send_error(result_state);
    }

    result_state = i2c_restart_send();
    // No error state yet

    result_state = i2c_send_sla(dev_addr + i2c_read);
    if (result_state != SEND_SLA_R_SUCC) {
        return i2c_send_error(result_state);
    }

    result_state = i2c_receive_data(buffer, length);
    // No error state yet

    if (0 == stop_send) {
        // stop_send == false
        stop_send_i2c();
    }
    curr_state = I2C_OK;
    return curr_state;
}

I2CState get_i2c_state() {
    return curr_state;
}

void attach_slave_rx_event(void (*function)(uint8_t *, int)) {
    on_slave_receive = function;
}

void attach_slave_tx_event(void (*function)(void)) {
    on_slave_transmit = function;
}
