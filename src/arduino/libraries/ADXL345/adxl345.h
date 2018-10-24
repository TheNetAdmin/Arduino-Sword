#ifndef _SWORD_ADXL345_H_
#define _SWORD_ADXL345_H_

#include <inttypes.h>

#define ADXLADDRESS 0x53

#define ADXLREG_DEVICEID 0x00      //器件ID
#define ADXLREG_THRERSHTAP 0x1D    //敲击阈值
#define ADXLREG_OFSX 0x1E          // X轴偏移
#define ADXLREG_OFSY 0x1F          // Y轴偏移
#define ADXLREG_OFSZ 0x20          // Z轴偏移
#define ADXLREG_DUR 0x21           //敲击持续时间
#define ADXLREG_LATENT 0x22        //敲击延迟
#define ADXREG_WINDOW 0x23         //敲击窗口
#define ADXLREG_THRESHACT 0x24     //活动阈值
#define ADXLREG_THRESHINACT 0x25   //静止阈值
#define ADXLREG_TIMEINACT 0x26     //静止时间
#define ADXLREG_ACTINACTCTL 0x27   //轴使能控制活动和静止检测
#define ADXLREG_THRESHFF 0x28      //自由落体阈值
#define ADXLREG_TIMEFF 0X29        //自由落体时间
#define ADXLREG_TAPAXES 0x2A       //单击/双击轴控制
#define ADXLREG_ACTTAPSTATUS 0x2B  //单击/双击源
#define ADXLREG_BWRATE 0x2C        //数据速率及功率模式控制
#define ADXLREG_POWERCTL 0x2D      //省电特性控制
#define ADXLREG_INTENABLE 0x2E     //中断使能控制
#define ADXLREG_INTMAP 0x2F        //中断映射控制
#define ADXLREG_INRSOURCE 0x30     //中断源
#define ADXLREG_DATAFORMAT 0x31    //数据格式控制
#define ADXLREG_DATAX0 0x32        // X轴数据0
#define ADXLREG_DATAX1 0x33        // X轴数据1
#define ADXLREG_DATAY0 0x34        // Y轴数据0
#define ADXLREG_DATAY1 0x35        // Y轴数据1
#define ADXLREG_DATAZ0 0X36        // Z轴数据0
#define ADXLREG_DATAZ1 0X37        // Z轴数据1
#define ADXLREG_FIFOCTL 0x38       // FIFO控制
#define ADXLREG_FIFOSTATUS 0x39    // FIF0状态

#define ADXLVALUE_DATAFORMAT 0x0B
#define ADXLVALUE_BWRATE 0x0E
#define ADXLVALUE_POWERCTL 0x08
#define ADXLVALUE_INTENABLE 0x80
#define ADXLVALUE_OFSX 0x00
#define ADXLVALUE_OFSY 0x00
#define ADXLVALUE_OFSZ 0x05

typedef struct  {
    int16_t X;
    int16_t Y;
    int16_t Z;
} Accelerate;

void begin_adxl345();
bool available_adxl345();
int16_t get_x_accelerate_adxl345();
int16_t get_y_accelerate_adxl345();
int16_t get_z_accelerate_adxl345();
Accelerate get_accelerate_adxl345();
uint8_t get_device_id_adxl345();


/*hardware i2c*/
void hw_begin_adxl345();
int16_t hw_get_x_accelerate_adxl345();
int16_t hw_get_y_accelerate_adxl345();
int16_t hw_get_z_accelerate_adxl345();

void hw_i2c_test_adxl345();

#endif // ! _SWORD_ADXL345_H_