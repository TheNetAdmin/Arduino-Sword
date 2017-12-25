#include "arch.h"
#include <vga.h>

unsigned int* const CHAR_VRAM = (unsigned int*)0xbfc04000;
unsigned int* const GRAPHIC_VRAM = (unsigned int*)0xbfe0000;
unsigned int* const GPIO_SWITCH = (unsigned int*)0xbfc09000;     // switch read-only
unsigned int* const GPIO_BUTTON = (unsigned int*)0xbfc09004;     // button read-only
unsigned int* const GPIO_SEG = (unsigned int*)0xbfc09008;        // Seg R/W
unsigned int* const GPIO_LED = (unsigned int*)0xbfc0900c;        // LED R/W
unsigned int* const GPIO_PS2_DATA = (unsigned int*)0xbfc09010;   // PS/2 data register, R/W
unsigned int* const GPIO_PS2_CTRL = (unsigned int*)0xbfc09014;   // PS/2 control register, R/W
unsigned int* const GPIO_UART_DATA = (unsigned int*)0xbfc09018;  // UART data register, R/W
unsigned int* const GPIO_UART_CTRL = (unsigned int*)0xbfc0901c;  // UART control register, R/W
unsigned int* const GPIO_CURSOR = (unsigned int*)0xbfc09020;     // Cursor 8-bit frequency 8-bit row 8-bit col
unsigned int* const VGA_MODE = (unsigned int*)0xbfc09024;        // enable graphic mode

// kernel sp
volatile unsigned int kernel_sp = 0x81000000;

// exception and interrupt

void exc_intr_handler(unsigned int status, unsigned int cause, context* pt_context) {
    unsigned bad_vaddr;
    asm volatile("mfc0 %0, $8\n\t" : "=r"(bad_vaddr));
    kernel_printf("\t Cause: 0x%x, Status: 0x%x, BadVaddr: 0x%x\n", cause, status, bad_vaddr);
    while (1);
}

void do_exceptions(unsigned int status, unsigned int cause, context* pt_context){
    kernel_printf("Exception:\n");
    exc_intr_handler(status, cause, pt_context);
}
void do_interrupts(unsigned int status, unsigned int cause, context* pt_context){
    kernel_printf("Interrupt:\n");
    exc_intr_handler(status, cause, pt_context);
}
void init_exception() {
    // status 0000 0000 0000 0000 0000 0000 0000 0000
    // cause 0000 0000 1000 0000 0000 0000 0000 0000
    asm volatile(
        "mtc0 $zero, $12\n\t"
        "li $t0, 0x800000\n\t"
        "mtc0 $t0, $13\n\t");
}