#ifndef _WIRE_H_
#define _WIRE_H_

#include <inttypes.h>

#define BUFFER_LENGTH 32
#define WIRE_HAS_END 1

void begin_wire();
void end_wire();

// addr = Device Address
void begin_transmission_wire(uint8_t addr);

// sendStop = 0 : do not stop send after this operation
// sendStop = 1 : stop send after this operation
uint8_t end_transmission_wire(uint8_t sendStop);

// Read `length` bytes data from `addr`
// Stop receive after this operation if `sendStop` = 1
uint8_t request_from_wire(uint8_t addr, uint8_t length, uint8_t sendStop);

// Write `data` into buffer
size_t write_wire_byte(uint8_t data);
// Write `length` bytes from `data` into buffer
size_t write_wire(const uint8_t * data, size_t length);

// Read 1 byte from buffer
int read_wire();

// Query how many bytes are ready to be read
int available_wire();

#endif  // ! _WIRE_H_