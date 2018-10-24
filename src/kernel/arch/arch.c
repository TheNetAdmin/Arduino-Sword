#include "arch.h"
#include <vga.h>

unsigned int* const CHAR_VRAM = (unsigned int*)0xbfc04000;
unsigned int* const GPIO_CURSOR = (unsigned int*)0xbfc09020;     // Cursor 8-bit frequency 8-bit row 8-bit col

// kernel sp
volatile unsigned int kernel_sp = 0x81000000;

#ifdef __cplusplus
extern "C" {
#endif

void do_exceptions(unsigned int status, unsigned int cause, context* pt_context) {
    printf("Exception:\n");
}
void do_interrupts(unsigned int status, unsigned int cause, context* pt_context) {
    printf("Interrupt:\n");
}
void init_exception() {
    // status 0000 0000 0000 0000 0000 0000 0000 0000
    // cause 0000 0000 1000 0000 0000 0000 0000 0000
    asm volatile(
        "mtc0 $zero, $12\n\t"
        "li $t0, 0x800000\n\t"
        "mtc0 $t0, $13\n\t");
}

#ifdef __cplusplus
}
#endif