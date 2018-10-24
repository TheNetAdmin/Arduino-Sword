#include <Arduino.h>
#include <arch.h>
#include <main.h>
#include <vga.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif