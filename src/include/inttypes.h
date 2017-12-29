#ifndef _SWORD_TYPES_H_
#define _SWORD_TYPES_H_

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed long int int32_t;

#define max_uint32_t ((uint32_t)0xffffffff)

typedef uint32_t size_t;

typedef uint8_t bool;
#define true ((uint8_t)1)
#define false ((uint8_t)0)

#endif // _SWORD_TYPES_H_