#include <Arduino.h>
#include <vga.h>
#include "i2c_hardware.h"
#include "../../cores/pins.h"


uint32_t* const EN = (uint32_t*)0xbfc09300;
uint32_t* const DEV_ADDR = (uint32_t*)0xbfc09304;
uint32_t* const CMD = (uint32_t*)0xbfc09308;
uint32_t* const LENGTH = (uint32_t*)0xbfc0930c;
uint32_t* const STATE = (uint32_t*)0xbfc09310;
uint32_t* const DATA = (uint32_t*)0xbfc09314;
uint32_t* const REG_ADDR = (uint32_t*)0xbfc09318;
uint32_t* const RESTART_HOLD_CYCLE = (uint32_t*)0xbfc0931c;

uint32_t* const EN_ADC = (uint32_t*)0xbfc09400;
uint32_t* const DEV_ADDR_ADC = (uint32_t*)0xbfc09404;
uint32_t* const CMD_ADC = (uint32_t*)0xbfc09408;
uint32_t* const LENGTH_ADC = (uint32_t*)0xbfc0940c;
uint32_t* const STATE_ADC = (uint32_t*)0xbfc09410;
uint32_t* const DATA_ADC = (uint32_t*)0xbfc09414;
uint32_t* const REG_ADDR_ADC = (uint32_t*)0xbfc09418;
uint32_t* const RESTART_HOLD_CYCLE_ADC = (uint32_t*)0xbfc0941c;

void global_init_i2c_h() { // only execute once
    set_pin_func(PIN_AIO4, UNO);
    set_pin_mode(PIN_AIO4, OutputOpenDrain);
    set_pin_func(PIN_AIO5, UNO);
    set_pin_mode(PIN_AIO5, OutputOpenDrain);
}
void init_i2c_h(){
    *EN = 0;
    *DEV_ADDR = 0;
    *CMD = 0;
    *LENGTH = 0;
    *STATE = READY;
    *DATA = 0;
    *REG_ADDR = 0;
    set_i2c_restart_hold_cycle(0);
}

void init_i2c_h_adc(){
    *EN_ADC = 0;
    *DEV_ADDR_ADC = 0;
    *CMD_ADC = 0;
    *LENGTH_ADC = 0;
    *STATE_ADC = READY;
    *DATA_ADC = 0;
    *REG_ADDR_ADC = 0;
    set_i2c_restart_hold_cycle_adc(0);
}

void set_i2c_restart_hold_cycle(uint32_t cycle){
    *RESTART_HOLD_CYCLE = cycle + 1;
}
void set_i2c_restart_hold_cycle_adc(uint32_t cycle){
    *RESTART_HOLD_CYCLE_ADC = cycle + 1;
}

void set_i2c_restart_hold_time(uint32_t us){
    // 1 cycle = 10.24 us
    if(us < 10){
        set_i2c_restart_hold_cycle(1);
    }
    else{
        set_i2c_restart_hold_cycle(us/10);
    }
}

void set_i2c_restart_hold_time_adc(uint32_t us){
    if(us < 10){
        set_i2c_restart_hold_cycle_adc(1);
    }
    else{
        set_i2c_restart_hold_cycle_adc(us/10);
    }
}

uint32_t read_i2c_h_ADC(uint8_t dev_addr, uint8_t reg_addr, uint8_t length){
    uint32_t data = 0;
    uint32_t last_state = READY;
    uint32_t now_state = READY;
    *DEV_ADDR_ADC =(uint32_t)dev_addr;
    *CMD_ADC = 1;
    *REG_ADDR_ADC = (uint32_t)reg_addr;
    *LENGTH_ADC =(uint32_t)length;
    *EN_ADC = 1;
    if(*EN_ADC==1){
        while((now_state = *STATE_ADC)!=I2C_OK){
            if(now_state==T_NACK&&last_state==now_state){
                printf("i2c: NACK during address transfer\n");
                break;
            }
            last_state=now_state;
        }
        data = *DATA_ADC;
        *EN_ADC = 0;
    }
    delay(100);
    return data;
}

uint32_t read_i2c_h(uint8_t dev_addr, uint8_t reg_addr, uint8_t length){
    uint32_t data = 0;
    uint32_t last_state = READY;
    uint32_t now_state = READY;
    *DEV_ADDR =(uint32_t)dev_addr;
    *CMD = 1;
    *REG_ADDR = (uint32_t)reg_addr;
    *LENGTH =(uint32_t)length;
    *EN = 1;
    if(*EN==1){
        while((now_state = *STATE)!=I2C_OK){
            if(now_state==T_NACK&&last_state==now_state){
                printf("i2c: NACK during address transfer\n");
                break;
            }
            last_state=now_state;
        }
        data = *DATA;
        *EN = 0;
    }
    delay(100);
    return data;
}

void write_i2c_h(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, const uint32_t data){
    uint32_t last_state = READY;
    uint32_t now_state = READY;
    *DEV_ADDR =(uint32_t)dev_addr;
    *CMD = 0;
    *REG_ADDR =(uint32_t)reg_addr;
    *LENGTH =(uint32_t)length;
    *DATA = data;
    *EN = 1;
    if(*EN==1){
        while((now_state = *STATE)!=I2C_OK){
            if(now_state==T_NACK&&last_state==now_state){
                printf("i2c: NACK during address transfer\n");
                break;
            }
            if(now_state==W_NACK&&last_state==now_state){
                printf("i2c: NACK during data transfer\n");
                break;
            }
            last_state=now_state;
        }
        *EN = 0;
    }
    delay(100);
}

void write_i2c_h_ADC( uint8_t dev_addr, uint8_t reg_addr, uint8_t length, const uint32_t data){
    uint32_t last_state = READY;
    uint32_t now_state = READY;
    *DEV_ADDR_ADC =(uint32_t)dev_addr;
    *CMD_ADC = 0;
    *REG_ADDR_ADC = (uint32_t)reg_addr;
    *LENGTH_ADC =(uint32_t)length;
    *DATA_ADC = data;
    *EN_ADC = 1;
    if(*EN_ADC==1){
        while((now_state = *STATE_ADC)!=I2C_OK){
            //printf("w-%d \t",*STATE );
            if(now_state==T_NACK&&last_state==now_state){
                printf("i2c: NACK during address transfer\n");
                break;
            }
            if(now_state==T_NACK&&last_state==now_state){
                printf("i2c: NACK during data transfer\n");
                break;
            }
            last_state=now_state;
        }
        *EN_ADC = 0;
    }
    delay(100);
}