#ifndef _DRIVER_VGA_H
#define _DRIVER_VGA_H

#include <utils.h>

extern int cursor_row;
extern int cursor_col;
extern int cursor_freq;

#define VGA_RED   0x00f
#define VGA_GREEN 0x0f0
#define VGA_BLUE  0xf00
#define VGA_BLACK 0x000
#define VGA_WHITE 0xfff

struct screen_color {
    int front;
    int background;
};

void init_vga();
void set_cursor();
void clear_screen(int row);
void scroll_screen();
void putchar_at(int ch, int fc, int bg, int row, int col);
int putchar(int ch, int fc, int bg);
int puts(const char* s, int fc, int bg);
int putint(int i, int fc, int bg);
int vprintf(const char *format, va_list ap);
int printf(const char* format, ...);

#endif // ! _DRIVER_VGA_H