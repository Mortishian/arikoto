#pragma once

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

void init_framebuffer(uint32_t *fb, size_t p, size_t bpp_val, size_t width, size_t height);
void putchar(char c, uint32_t color);
void puts(const char *str, uint32_t color);
