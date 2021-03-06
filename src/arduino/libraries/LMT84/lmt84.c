#include "lmt84.h"
#include <inttypes.h>
#include <Arduino.h>
#include <vga.h>

#define MAP_SIZE 200

temperature temp_map[MAP_SIZE] = {
    {1299, -50}, {1294, -49}, {1289, -48}, {1284, -47}, {1278, -46}, {1273, -45}, {1268, -44}, {1263, -43}, {1257, -42}, {1252, -41},
    {1247, -40}, {1242, -39}, {1236, -38}, {1231, -37}, {1226, -36}, {1221, -35}, {1215, -34}, {1210, -33}, {1205, -32}, {1200, -31},
    {1194, -30}, {1189, -29}, {1184, -28}, {1178, -27}, {1173, -26}, {1168, -25}, {1162, -24}, {1157, -23}, {1152, -22}, {1146, -21},
    {1141, -20}, {1136, -19}, {1130, -18}, {1125, -17}, {1120, -16}, {1114, -15}, {1109, -14}, {1104, -13}, {1098, -12}, {1093, -11},
    {1088, -10}, {1082, -9},  {1077, -8},  {1072, -7},  {1066, -6},  {1061, -5},  {1055, -4},  {1050, -3},  {1044, -2},  {1039, -1},
    {1034, 0},   {1028, 1},   {1023, 2},   {1017, 3},   {1012, 4},   {1007, 5},   {1001, 6},   {996, 7},    {990, 8},    {985, 9},
    {980, 10},   {974, 11},   {969, 12},   {963, 13},   {958, 14},   {952, 15},   {947, 16},   {941, 17},   {936, 18},   {931, 19},
    {925, 20},   {920, 21},   {914, 22},   {909, 23},   {903, 24},   {898, 25},   {892, 26},   {887, 27},   {882, 28},   {876, 29},
    {871, 30},   {865, 31},   {860, 32},   {854, 33},   {849, 34},   {843, 35},   {838, 36},   {832, 37},   {827, 38},   {821, 39},
    {816, 40},   {810, 41},   {804, 42},   {799, 43},   {793, 44},   {788, 45},   {782, 46},   {777, 47},   {771, 48},   {766, 49},
    {760, 50},   {754, 51},   {749, 52},   {743, 53},   {738, 54},   {732, 55},   {726, 56},   {721, 57},   {715, 58},   {710, 59},
    {704, 60},   {698, 61},   {693, 62},   {687, 63},   {681, 64},   {676, 65},   {670, 66},   {664, 67},   {659, 68},   {653, 69},
    {647, 70},   {642, 71},   {636, 72},   {630, 73},   {625, 74},   {619, 75},   {613, 76},   {608, 77},   {602, 78},   {596, 79},
    {591, 80},   {585, 81},   {579, 82},   {574, 83},   {568, 84},   {562, 85},   {557, 86},   {551, 87},   {545, 88},   {539, 89},
    {534, 90},   {528, 91},   {522, 92},   {517, 93},   {511, 94},   {505, 95},   {499, 96},   {494, 97},   {488, 98},   {482, 99},
    {476, 100},  {471, 101},  {465, 102},  {459, 103},  {453, 104},  {448, 105},  {442, 106},  {436, 107},  {430, 108},  {425, 109},
    {419, 110},  {413, 111},  {407, 112},  {401, 113},  {396, 114},  {390, 115},  {384, 116},  {378, 117},  {372, 118},  {367, 119},
    {361, 120},  {355, 121},  {349, 122},  {343, 123},  {337, 124},  {332, 125},  {326, 126},  {320, 127},  {314, 128},  {308, 129},
    {302, 130},  {296, 131},  {291, 132},  {285, 133},  {279, 134},  {273, 135},  {267, 136},  {261, 137},  {255, 138},  {249, 139},
    {243, 140},  {237, 141},  {231, 142},  {225, 143},  {219, 144},  {213, 145},  {207, 146},  {201, 147},  {195, 148},  {189, 149}};

int32_t get_temperature_lmt84(int32_t voltage) {
    int map_index = MAP_SIZE - 1;
    for (int i = 0; i < MAP_SIZE; i++) {
        if (voltage > temp_map[i].voltage) {
            map_index = i;
            break;
        }
    }
    return temp_map[map_index].celsius;
}

/*read analog signal from lmt84 by hardware i2c and i2c communicate with adc*/
void test_lmt84() { // adc
    uint32_t result = 0;
    while(1) {
        result = analogRead(PIN_AIO0); // communicata with adc
        result = result * 3300 / 1023;
        result = get_temperature_lmt84(result);
        printf("Original temperature data: %d\n", result);
        delay(2000);
   }
}
