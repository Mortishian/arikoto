#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

// Color structure for RGB values
struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// Function declarations
void framebuffer_init(void);
void draw_pixel(int x, int y, struct color col);
void draw_char(int x, int y, char c, struct color col);
void log_to_framebuffer(int x, int y, const char *text);

#endif // FRAMEBUFFER_H
