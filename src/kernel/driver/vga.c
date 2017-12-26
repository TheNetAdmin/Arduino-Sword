#include "vga.h"
#include <arch.h>
#include <utils.h>

const int VGA_CHAR_MAX_ROW = 32;
const int VGA_CHAR_MAX_COL = 128;
const int VGA_CHAR_ROW = 30;
const int VGA_CHAR_COL = 80;
int cursor_row;
int cursor_col;
int cursor_freq = 31;

void set_cursor() {
    *GPIO_CURSOR = ((cursor_freq & 0xff) << 16) + ((cursor_row & 0xff) << 8) + (cursor_col & 0xff);
}

void init_vga() {
    unsigned int w = 0x000fff00;
    cursor_row = cursor_col = 0;
    cursor_freq = 31;
    set_cursor();
}

void clear_screen(int scope) {
    unsigned int w = 0x000fff00;
    scope &= 31;
    cursor_col = 0;
    cursor_row = 0;
    set_cursor();
    memset_word(CHAR_VRAM, w, scope * VGA_CHAR_MAX_COL);
}

void scroll_screen() {
    unsigned int w = 0x000fff00;
    memcpy(CHAR_VRAM, (CHAR_VRAM + VGA_CHAR_MAX_COL), (VGA_CHAR_ROW - 2) * VGA_CHAR_MAX_COL * 4);
    memset_word((CHAR_VRAM + (VGA_CHAR_ROW - 2) * VGA_CHAR_MAX_COL), w, VGA_CHAR_MAX_COL);
}

void putchar_at(int ch, int fc, int bg, int row, int col) {
    unsigned int *p;
    row = row & 31;
    col = col & 127;
    p = CHAR_VRAM + row * VGA_CHAR_MAX_COL + col;
    *p = ((bg & 0xfff) << 20) + ((fc & 0xfff) << 8) + (ch & 0xff);
}

int putchar(int ch, int fc, int bg) {
    unsigned int w = 0x000fff00;
    if (ch == '\r')
        return ch;
    if (ch == '\n') {
        memset_word(CHAR_VRAM + cursor_row * VGA_CHAR_MAX_COL + cursor_col, w, VGA_CHAR_COL - cursor_col);
        cursor_col = 0;
        if (cursor_row == VGA_CHAR_ROW - 2) {
            scroll_screen();
        } else {
            cursor_row++;
#ifdef VGA_CALIBRATE
            putchar(' ', fc, bg);
#endif  // VGA_CALIBRATE
        }
    } else if (ch == '\t') {
        if (cursor_col >= VGA_CHAR_COL - 4) {
            putchar('\n', 0, 0);
        } else {
            memset_word(CHAR_VRAM + cursor_row * VGA_CHAR_MAX_COL + cursor_col, w, 4 - cursor_col & 3);
            cursor_col = (cursor_col + 4) & (-4);
        }
    } else {
        if (cursor_col == VGA_CHAR_COL) {
            putchar('\n', 0, 0);
        }
        putchar_at(ch, fc, bg, cursor_row, cursor_col);
        cursor_col++;
    }
    set_cursor();
    return ch;
}

int puts(const char *s, int fc, int bg) {
    int ret = 0;
    while (*s) {
        ret++;
        putchar(*s++, fc, bg);
    }
    return ret;
}

int putint(int x, int fc, int bg) {
    char buffer[12];
    char *ptr = buffer + 11;
    int neg = 0;
    buffer[11] = 0;
    if (x == 0) {
        putchar('0', fc, bg);
        return x;
    }
    if (x < 0) {
        neg = 1;
        x = -x;
    }
    while (x) {
        ptr--;
        *ptr = (x % 10) + '0';
        x /= 10;
    }
    if (neg) {
        ptr--;
        *ptr = '-';
    }
    puts(ptr, fc, bg);
    return x;
}

static const char *HEX_MAP = "0123456789abcdef";
int putintx(unsigned int x, int fc, int bg) {
    char buffer[12];
    char *ptr = buffer + 11;
    buffer[11] = 0;
    if (x == 0) {
        putchar('0', fc, bg);
        return x;
    }
    while (x) {
        ptr--;
        *ptr = HEX_MAP[x & 15];
        x >>= 4;
    }
    puts(ptr, fc, bg);
    return x;
}

int vprintf(const char *format, va_list ap) {
    int cnt = 0;
    while (*format) {
        if (*format != '%') {
            putchar(*format++, 0xfff, 0);
        } else {
            format++;
            switch (*format) {
                case 'c': {
                    char valch = va_arg(ap, int);
                    putchar(valch, 0xfff, 0);
                    format++;
                    cnt++;
                    break;
                }
                case 'd': {
                    int valint = va_arg(ap, int);
                    putint(valint, 0xfff, 0);
                    format++;
                    cnt++;
                    break;
                }
                case 'x': {
                    int valint = va_arg(ap, int);
                    putintx(valint, 0xfff, 0);
                    format++;
                    cnt++;
                    break;
                }
                case 's': {
                    char *valstr = va_arg(ap, char *);
                    puts(valstr, 0xfff, 0);
                    format++;
                    cnt++;
                    break;
                }
                default: {
                    cnt = -1;
                    goto exit;
                }
            }
        }
    }
exit:
    return cnt;
}

int printf(const char *format, ...) {
    int cnt = 0;
    va_list ap;
    va_start(ap, format);
    cnt = vprintf(format, ap);
    va_end(ap);
    return cnt;
}
