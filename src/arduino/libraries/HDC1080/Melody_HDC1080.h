#ifndef __MELODY_HDC1080_H__
#define __MELODY_HDC1080_H__

#define HDC_ADDRESS 0x40  // address

#define HDCREG_TEMPERATURE 0x00     // Temperature measurement output
#define HDCREG_HUMIDITY 0x01        // Relative Humidity measurement output
#define HDCREG_CONFIGURATION 0x02   // configuration and status
#define HDCREG_SERIALID_FRIST 0xFB  // First 2 bytes of the serial of the part
#define HDCREG_SERIALID_MID 0xFC    // Mid 2 bytes of the serial of the part
#define HDCREG_SERIALID_LAST 0xFD   // Last 2 bytes of the serial of the part
#define HDCREG_MANUFACTURERID 0xFE  // ID of TI
#define HDCREG_DEVICEID 0xFF        // ID of the device

#define HDC_CONFIGURATION \
    0x0000  // Normal Operation,Temperature or Humidity is acquired
            // Measurement Resolution:14bit

// public:
void begin_hdc1080();
uint16_t getTemperature_hdc1080();
uint16_t getHumidity_hdc1080();
uint16_t getManufacturerID_hdc1080();
uint16_t getDeviceID_hdc1080();
uint16_t* getSerialID_hdc1080(uint16_t* pdata, uint8_t length);

#endif /*__MELODY_HDC1080_H__*/