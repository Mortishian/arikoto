#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <print.h>
#include <kernel.h>

extern uint8_t _binary_matrix_psf_start;
extern uint8_t _binary_matrix_psf_size;

// PSF1 font header structure.
struct psf1_header {
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charsize;
};

static uint32_t *framebuffer = NULL;
static size_t pitch = 0;
static size_t bpp = 0;
static size_t cursor_x = 0;
static size_t cursor_y = 0;
static size_t screen_width = 0;
static size_t screen_height = 0;

void init_framebuffer(uint32_t *fb, size_t p, size_t bpp_val, size_t width, size_t height) {
    framebuffer = fb;
    pitch = p;
    bpp = bpp_val;
    screen_width = width;
    screen_height = height;
    cursor_x = 0;
    cursor_y = 0;
}

// Function to render a single character.
void putchar(char c, uint32_t color) {
    struct psf1_header *font = (struct psf1_header *)&_binary_matrix_psf_start;
    uint8_t *glyphs = (uint8_t *)(&_binary_matrix_psf_start + sizeof(struct psf1_header));
    uint8_t *glyph = glyphs + (c * font->charsize);

    for (size_t py = 0; py < 16; py++) {
        for (size_t px = 0; px < 8; px++) {
            size_t pixel_index = (cursor_y + py) * (pitch / (bpp / 8)) + (cursor_x + px);
            framebuffer[pixel_index] = COLOR_BLACK;
        }
    }

    for (size_t py = 0; py < font->charsize; py++) {
        for (size_t px = 0; px < 8; px++) {
            if (glyph[py] & (0x80 >> px)) {
                size_t pixel_index = (cursor_y + py) * (pitch / (bpp / 8)) + (cursor_x + px);
                framebuffer[pixel_index] = color;
            }
        }
    }
}

// Function to render a string with automatic line breaks and overwriting text
void puts(const char *str, uint32_t color) {
    while (*str) {
        if (*str == '\n' || cursor_x + 8 >= screen_width) {
            cursor_x = 0; cursor_y += 16;
        }

        if (*str != '\n') {
            putchar(*str, color);
            cursor_x += 8;
        }

        // Reset to top of screen if exceeding height
        if (cursor_y + 16 >= screen_height) cursor_y = 0;

        str++;
    }

    // After string, move to next line
    cursor_x = 0; cursor_y += 16;
}
