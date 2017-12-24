#ifndef _WIRE_H_
#define _WIRE_H_

#include <inttypes.h>

#define BUFFER_LENGTH 32

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

/*
beginTransmission(int)
endTransmission(void)
begin(void)
write(uint8_t)
write(uint8_t*, uint8_t)
requestFrom(int, int)
available(void)
read(void)
*/

class TwoWire {
private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;

public:
    TwoWire();

    void begin();

    void end();

    void beginTransmission(uint8_t);

    uint8_t endTransmission();

    uint8_t endTransmission(uint8_t);

    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);

    uint8_t requestFrom(uint8_t, uint8_t);

    uint8_t requestFrom(int, int);

    virtual size_t write(uint8_t);

    virtual size_t write(const uint8_t *, size_t);

    virtual int available();

    virtual int read();

};

extern TwoWire Wire;

#endif // ! _WIRE_H_