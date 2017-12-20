#ifndef _I2C_PRIVATE_H_
#define _I2C_PRIVATE_H_

#include <cores/types.h>

void i2c_stop_send();
typedef enum {
    DEFAULT = 0,
    // bus status
    BUS_ERROR,
    // start_send()
    START_FAIL_BUSY,
    START_SUCC,
    // restart_send()
    RESTART_END,
    // send_byte()
    SEND_BYTE_FAIL,
    SEND_BYTE_SUCC,
    SEND_BYTE_NACK,
    // send_sla()
    SEND_SLA_R_SUCC,
    SEND_SLA_W_SUCC,
    SEND_SLA_R_NACK,
    SEND_SLA_W_NACK,
    // send_data()
    SEND_DATA_NACK,
    SEND_DATA_SUCC,
    SEND_DATA_FAIL,
    // receive_data()
    RECV_DATA_END
}I2CResult;

I2CState i2c_send_error(I2CResult state);
I2CResult i2c_start_send();

I2CResult i2c_send_byte(uint8_t data);
I2CResult i2c_send_sla(uint8_t d_addr);
I2CResult i2c_send_data(const uint8_t* buffer, uint32_t length);
I2CResult i2c_restart_send();
I2CResult i2c_receive_data(uint8_t* buffer, uint32_t length);

#endif //_I2C_PRIVATE_H_
