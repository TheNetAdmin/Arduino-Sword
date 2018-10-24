#include <utils.h>
#include <vga.h>

void* memcpy(void* dest, void* src, int len) {
    char* deststr = (char*)dest;
    char* srcstr = (char*)src;
    while (len--) {
        *deststr = *srcstr;
        deststr++;
        srcstr++;
    }
    return dest;
}

void* memset(void* dest, int b, int len) {
    char content = b ? -1 : 0;
    char* deststr = (char*)dest;
    while (len--) {
        *deststr = content;
        deststr++;
    }
    return dest;
}

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
