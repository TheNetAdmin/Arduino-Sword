#include <arch.h>
#include <vga.h>
#include <main.h>
#include <Arduino.h>

void init_kernel() {
    kernel_clear_screen(31);
    init_exception();
    init_vga();
    // Arduino
    init_arduino();
    // run arduino
    main();
    while (1);
}
