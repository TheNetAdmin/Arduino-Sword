#ifndef _WIRE_H_
#define _WIRE_H_

#include <inttypes.h>

#define BUFFER_LENGTH 32

#define WIRE_HAS_END 1

void begin_wire();

void end_wire();

void begin_transmission_wire(uint8_t);

uint8_t end_transmission_wire(uint8_t);

uint8_t request_from_wire(uint8_t, uint8_t, uint8_t);

size_t write_wire_byte(uint8_t);

size_t write_wire(const uint8_t *, size_t);

int available_wire();

int read_wire();

#endif  // ! _WIRE_H_