#include "framebuffer.h"
#include <limine.h>
#include <stddef.h>

// Limine framebuffer request
__attribute__((used, section(".requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Framebuffer info
volatile uint32_t *fb_ptr;
int width, height, pitch, bpp;

// Initialize framebuffer
void framebuffer_init(void) {
    // Check Limine response for framebuffer
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        for (;;) { asm("hlt"); }
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    fb_ptr = framebuffer->address;
    width = framebuffer->width;
    height = framebuffer->height;
    pitch = framebuffer->pitch;
    bpp = framebuffer->bpp;
}

// Basic function to draw a pixel
void draw_pixel(int x, int y, struct color col) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        fb_ptr[y * (pitch / 4) + x] = (col.r << 16) | (col.g << 8) | col.b;
    }
}

// Corrected 8x8 font data (aligned for HELLO!)
static const uint8_t font[128][8] = {
    // 'H' = 0x48 (Fixed)
    [72] = { 0x81, 0x81, 0x81, 0xFF, 0xFF, 0x81, 0x81, 0x81 },
    // 'E' = 0x45
    [69] = { 0xFF, 0x80, 0x80, 0xFE, 0x80, 0x80, 0xFF, 0x00 },
    // 'L' = 0x4C
    [76] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0x00 },
    // 'O' = 0x4F
    [79] = { 0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF, 0x00 },
    // '!' = 0x21
    [33] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00 }
};

// Function to draw a single character (8x8 font)
void draw_char(int x, int y, char c, struct color col) {
    for (int i = 0; i < 8; i++) {
        uint8_t row = font[(int)c][i];
        for (int j = 0; j < 8; j++) {
            if (row & (1 << (7 - j))) { // Left shift for proper character alignment
                draw_pixel(x + j, y + i, col);
            }
        }
    }
}

// Function to log text on the framebuffer
void log_to_framebuffer(int x, int y, const char *text) {
    struct color colors[] = {
        {255, 255, 255}, // White for 'H'
        {255, 255, 255}, // White for 'E'
        {255, 255, 255}, // White for 'L'
        {255, 255, 255}, // White for 'L'
        {255, 255, 255}, // White for 'O'
        {255, 255, 255}  // White for '!'
    };

    for (int i = 0; text[i] != '\0'; i++) {
        draw_char(x + i * 9, y, text[i], colors[i]);
    }
}
