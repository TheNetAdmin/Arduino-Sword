#include "i2c.h"
#include <Arduino.h>
#include <vga.h>
#include "../../cores/pins.h"
#include "i2c-private.h"

uint8_t I2C_SDA = PIN_AIO4;
uint8_t I2C_SCL = PIN_AIO5;

I2CState curr_state;
const uint32_t i2c_time = 4;
const uint8_t i2c_read = 0x01;
const uint8_t i2c_write = 0x00;

void set_pins_i2c(uint8_t sda, uint8_t scl) {
    I2C_SDA = sda;
    I2C_SCL = scl;
}

uint8_t get_sda_i2c() {
    return I2C_SDA;
}

uint8_t get_scl_i2c() {
    return I2C_SCL;
}

void stop_send_i2c() {
    pinMode(I2C_SCL, OUTPUT);
    digitalWrite(I2C_SCL, LOW);
    pinMode(I2C_SDA, OUTPUT);
    digitalWrite(I2C_SDA, LOW);
    wait_usec(i2c_time);

    pinMode(I2C_SCL, INPUT);
    while (!digitalRead(I2C_SCL)) {
    }
    wait_usec(1);

    pinMode(I2C_SDA, INPUT);
}

I2CResult i2c_start_send() {
    uint32_t i = 0;
    pinMode(I2C_SDA, INPUT);
    pinMode(I2C_SCL, INPUT);
    wait_usec(100);

    while (i < 50) {
        if (!digitalRead(I2C_SDA)) {
            return START_FAIL_BUSY;
        }
        if (!digitalRead(I2C_SCL)) {
            return START_FAIL_BUSY;
        }
        wait_usec(10);
        i++;
    }

    pinMode(I2C_SDA, OUTPUT);
    digitalWrite(I2C_SDA, LOW);
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
        pinMode(I2C_SCL, OUTPUT);
        digitalWrite(I2C_SCL, LOW);
        if (data & (0x01 << (7 - i))) {
            // send high
            if (!digitalRead(I2C_SDA)) {
                pinMode(I2C_SDA, INPUT);
            }
            pin_state = 1;
        } else {
            // send low
            if (digitalRead(I2C_SDA)) {
                pinMode(I2C_SDA, OUTPUT);
                digitalWrite(I2C_SDA, LOW);
            }
            pin_state = 0;
        }
        wait_usec(i2c_time);

        // I2C_SCL H transmit data
        pinMode(I2C_SCL, INPUT);
        while (!digitalRead(I2C_SCL)) {
        }
        if (pin_state != digitalRead(I2C_SDA)) {
            return SEND_BYTE_FAIL;
        }
        wait_usec(i2c_time);
    }

    // check ack/nack
    // scl low
    pinMode(I2C_SCL, OUTPUT);
    digitalWrite(I2C_SCL, LOW);
    pinMode(I2C_SDA, INPUT);
    wait_usec(i2c_time);
    // scl high
    pinMode(I2C_SCL, INPUT);
    while (!digitalRead(I2C_SCL)) {
    }
    wait_usec(i2c_time);
    if (!digitalRead(I2C_SDA)) {
        return SEND_BYTE_SUCC;
    } else {
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
    pinMode(I2C_SCL, INPUT);
    pinMode(I2C_SDA, INPUT);
    curr_state = I2C_OK;
}

void bus_clear() {
    pinMode(I2C_SDA, INPUT);
    for (int i = 0; i < 9; ++i) {
        pinMode(I2C_SCL, OUTPUT);
        digitalWrite(I2C_SCL, LOW);
        wait_usec(i2c_time);
        pinMode(I2C_SCL, INPUT);
        while (!digitalRead(I2C_SCL)) {
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
    printf("i2c_send_error(),%d\n",state);
    while (1)
        ;
    stop_send_i2c();
    return curr_state;
}

I2CResult i2c_restart_send() {
    // I2C_SCL LOW send start condition
    pinMode(I2C_SCL, OUTPUT);
    digitalWrite(I2C_SCL, LOW);
    pinMode(I2C_SDA, INPUT);
    wait_usec(i2c_time);

    // set start condition
    pinMode(I2C_SCL, INPUT);
    while (!digitalRead(I2C_SCL)) {
    }
    pinMode(I2C_SDA, OUTPUT);
    digitalWrite(I2C_SDA, LOW);
    wait_usec(i2c_time);

    return RESTART_END;
}

I2CResult i2c_receive_data(uint8_t *buffer, uint32_t length) {
    for (int i = 0; i < length; ++i) {
        *buffer = 0x00;
        pinMode(I2C_SCL, OUTPUT);
        digitalWrite(I2C_SCL, LOW);
        pinMode(I2C_SDA, INPUT);
        wait_usec(i2c_time);

        // read bit by bit
        for (int j = 0; j < 8; ++j) {
            // I2C_SCL HIGH
            pinMode(I2C_SCL, INPUT);
            while (!digitalRead(I2C_SCL)) {
            }
            *buffer |= digitalRead(I2C_SDA) << (7 - j);
            wait_usec(i2c_time);

            // I2C_SCL LOW
            pinMode(I2C_SCL, OUTPUT);
            digitalWrite(I2C_SCL, LOW);
            wait_usec(i2c_time);
        }

        // next byte
        ++buffer;

        if (i == length - 1) {
            // I2C_SCL HIGH
            wait_usec(i2c_time);
            pinMode(I2C_SCL, INPUT);
            while (!digitalRead(I2C_SCL)) {
            };
            wait_usec(i2c_time);
            break;
        } else {
            pinMode(I2C_SDA, OUTPUT);
            digitalWrite(I2C_SDA, LOW);
            wait_usec(i2c_time);
            // I2C_SCL HIGH
            pinMode(I2C_SCL, INPUT);
            while (!digitalRead(I2C_SCL)) {
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
    printf("0");
    if (result_state != START_SUCC) {
        return i2c_send_error(result_state);
    }
    printf("1");
    result_state = i2c_send_sla((dev_addr << 1) | i2c_write);
    if (result_state != SEND_SLA_W_SUCC) {
        i2c_send_error(result_state);
    }
    printf("2");
    result_state = i2c_send_data(data, length);
    if (result_state != SEND_DATA_SUCC) {
        i2c_send_error(result_state);
    }
    printf("3");
    if (true == send_stop) {
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
    
    result_state = i2c_send_sla((dev_addr << 1) | i2c_read);
    if (result_state != SEND_SLA_R_SUCC) {
        return i2c_send_error(result_state);
    }

    result_state = i2c_receive_data(buffer, length);
    // No error state yet

    if (true == stop_send) {
        stop_send_i2c();
    }
    curr_state = I2C_OK;
    return curr_state;
}

I2CState get_i2c_state() {
    return curr_state;
}
