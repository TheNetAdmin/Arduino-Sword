#ifndef _I2C_HARDWARE_H_
#define _I2C_HARDWARE_H_

#include <inttypes.h>

#ifndef _I2C_STATUS
#define _I2C_STATUS
typedef enum { 
    I2C_OK = 0, 
    W_NACK,
    T_NACK,
    READY,
    START,
    ADDR,
    WAIT_ACK,
    READ,
    WRITE,
    W_ACK,
    RESTART,
    STOP
}I2CState_h;
#endif

extern uint32_t* const EN;
extern uint32_t* const DEV_ADDR;
extern uint32_t* const CMD;
extern uint32_t* const LENGTH;
extern uint32_t* const STATE;
extern uint32_t* const DATA;
extern uint32_t* const REG_ADDR;
extern uint32_t* const RESTART_HOLD_CYCLE;

extern uint32_t* const EN_ADC;
extern uint32_t* const DEV_ADDR_ADC;
extern uint32_t* const CMD_ADC;
extern uint32_t* const LENGTH_ADC;
extern uint32_t* const STATE_ADC;
extern uint32_t* const DATA_ADC;
extern uint32_t* const REG_ADDR_ADC;
extern uint32_t* const RESTART_HOLD_CYCLE_ADC;

uint32_t read_i2c_h(uint8_t dev_addr, uint8_t reg_addr, uint8_t length);
uint32_t read_i2c_h_ADC(uint8_t dev_addr,uint8_t reg_addr, uint8_t length);

void write_i2c_h(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, const uint32_t data);
void write_i2c_h_ADC( uint8_t dev_addr, uint8_t reg_addr, uint8_t length, const uint32_t data);

void global_init_i2c_h();
void init_i2c_h();
void init_i2c_h_adc();

void set_i2c_restart_hold_time(uint32_t us);
void set_i2c_restart_hold_time_adc(uint32_t us);

void set_i2c_restart_hold_cycle(uint32_t cycle);
void set_i2c_restart_hold_cycle_adc(uint32_t cycle);
#endif  // ! _I2C_HARDWARE_H_