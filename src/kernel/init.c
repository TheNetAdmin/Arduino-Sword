#include <Arduino.h>
#include <arch.h>
#include <main.h>
#include <vga.h>

void init_kernel() {
    clear_screen(31);
    init_exception();
    init_vga();
    // Arduino
    init_arduino();
    // run arduino
    main();
    while (1)
        ;
}
