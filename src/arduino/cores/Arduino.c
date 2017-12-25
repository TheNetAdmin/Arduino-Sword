#include <Arduino.h>
#include "pins.h"
#include "../libraries/Wire/Wire.h"
#include <vga.h>

void init_arduino() {
    kernel_clear_screen(31);
    init_pin_map();
    init_pin_gpio();
    delay(5000);
}

void delay(uint32_t ms) {
    uint32_t time = ms << 13;
    while (time--)
        ;
}

void wait_usec(uint32_t usec) {
    register int t = usec * 25;
    asm volatile(
        ".set noreorder\n\t"
        "wait_usec_loop:\n\t"
        "nop\n\t"
        "nop\n\t"
        "bne %0, $zero, wait_usec_loop\n\t"
        "addiu %0, %0, -1\n\t"
        ".set reorder\n\t" ::"r"(t));
}