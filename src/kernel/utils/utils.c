#include <utils.h>
#include <vga.h>

void* memcpy(void* dest, void* src, int len) {
    char* deststr = dest;
    char* srcstr = src;
    while (len--) {
        *deststr = *srcstr;
        deststr++;
        srcstr++;
    }
    return dest;
}

#pragma GCC push_options
#pragma GCC optimize("O2")
void* memset(void* dest, int b, int len) {
    char content = b ? -1 : 0;
    char* deststr = dest;
    while (len--) {
        *deststr = content;
        deststr++;
    }
    return dest;
}
#pragma GCC pop_options

unsigned int* memset_word(unsigned int* dest, unsigned int w, int len) {
    while (len--)
        *dest++ = w;

    return dest;
}

int strcmp(const char* dest, const char* src) {
    while ((*dest == *src) && (*dest != 0)) {
        dest++;
        src++;
    }
    return *dest - *src;
}

char* strcpy(char* dest, const char* src) {
    while ((*dest++ = *src++))
        ;
    return dest;
}

int pow(int x, int z) {
    int ret = 1;
    if (z < 0)
        return -1;
    while (z--) {
        ret *= x;
    }
    return ret;
}

#pragma GCC push_options
#pragma GCC optimize("O0")

void cache(unsigned int block_index) {
    block_index = block_index | 0x80000000;
    asm volatile(
        "li $t0, 233\n\t"
        "mtc0 $t0, $8\n\t"
        "move $t0, %0\n\t"
        "cache 0, 0($t0)\n\t"
        "nop\n\t"
        "cache 1, 0($t0)\n\t"
        : "=r"(block_index));
}

#pragma GCC pop_options

void serial_puts(char* str) {
    while (*str)
        *((unsigned int*)0xbfc09018) = *str++;
}

void serial_putc(char c) {
    *((unsigned int*)0xbfc09018) = c;
}

unsigned int is_bound(unsigned int val, unsigned int bound) {
    return !(val & (bound - 1));
}
