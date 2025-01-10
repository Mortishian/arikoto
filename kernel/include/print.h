#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include <stddef.h>

// Predefined color constants (RGB with 8-bit per channel)
#define COLOR_BLACK   0x000000
#define COLOR_WHITE   0xFFFFFF
#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_YELLOW  0xFFFF00
#define COLOR_CYAN    0x00FFFF
#define COLOR_MAGENTA 0xFF00FF

extern uint8_t _binary_matrix_psf_start;
extern uint8_t _binary_matrix_psf_size;

void putchar(uint32_t *fb, size_t pitch, size_t bpp, size_t x, size_t y, char c, uint32_t color);
void puts(uint32_t *fb, size_t pitch, size_t bpp, size_t *x, size_t *y, const char *str, uint32_t color, size_t max_width, size_t screen_height);

#endif
