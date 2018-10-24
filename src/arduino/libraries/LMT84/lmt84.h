#ifndef _LIB_LMT84_H_
#define _LIB_LMT84_H_

#include <inttypes.h>

typedef struct {
    int32_t voltage;
    int32_t celsius;
} temperature;

int32_t get_temperature_lmt84(int32_t voltage);
void test_lmt84();

#endif  // ! _LIB_LMT84_H_