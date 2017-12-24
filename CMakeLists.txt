cmake_minimum_required(VERSION 3.9)
project(Arduino_SWORD)

set(CMAKE_C_STANDARD 99)

add_executable(Arduino_SWORD
        src/cores/analog_rw.c
        src/cores/Arduino.c
        src/cores/Arduino.h
        src/cores/digital_rw.c
        src/cores/pins.c
        src/cores/pins.h
        src/include/inttypes.h
        src/libraries/I2C/i2c.c
        src/libraries/I2C/i2c.h
        src/libraries/I2C/i2c-private.h
        src/libraries/Wire/Wire.cpp
        src/libraries/Wire/Wire.h
        )

set(CMAKE_C_FLAGS "-nostdlib -nostdinc")
set(CMAKE_CXX_FLAGS "-nostdlib -nostdinc")

target_include_directories(Arduino_SWORD PUBLIC src src/include)
