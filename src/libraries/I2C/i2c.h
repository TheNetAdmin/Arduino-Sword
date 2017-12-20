#ifndef _I2C_H_
#define _I2C_H_

#include <inttypes.h>

typedef enum { 
    I2C_OK = 0,
    SLAW_NACK,
    SLAR_NACK,
    T_DATA_NACK,
    I2C_NO_INIT,
    I2C_BUSY,
    I2C_TIME_OUT,
    I2C_BUS_ERROR
} I2CState;

void init_i2c();

void bus_clear();

I2CState write_i2c(uint8_t d_addr, uint8_t r_addr, const uint8_t* buffer, uint32_t length);

I2CState read_i2c(uint8_t d_addr, uint8_t r_addr, uint8_t* buffer, uint32_t length);

I2CState read_i2c_simple(uint8_t d_addr, uint8_t *buffer, uint32_t length);

I2CState get_i2c_state();

#endif  // ! _I2C_H_