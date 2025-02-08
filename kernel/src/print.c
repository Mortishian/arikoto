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

// Function to render a single character.
void putchar(uint32_t *fb, size_t pitch, size_t bpp, size_t x, size_t y, char c, uint32_t color) {
    struct psf1_header *font = (struct psf1_header *)&_binary_matrix_psf_start;
    uint8_t *glyphs = (uint8_t *)(&_binary_matrix_psf_start + sizeof(struct psf1_header));

    uint8_t *glyph = glyphs + (c * font->charsize);

    for (size_t py = 0; py < font->charsize; py++) {
        for (size_t px = 0; px < 8; px++) {
            if (glyph[py] & (0x80 >> px)) {
                size_t pixel_index = (y + py) * (pitch / (bpp / 8)) + (x + px);
                fb[pixel_index] = color;
            }
        }
    }
}

// Function to render a string with automatic line breaks and overwriting text
void puts(uint32_t *fb, size_t pitch, size_t bpp, size_t *x, size_t *y,
          const char *str, uint32_t color, size_t screen_width, size_t screen_height) {
    while (*str) {
        if (*str == '\n' || *x + 8 >= screen_width) {
            *x = 0; *y += 16;  // Move to next line
        }

        if (*str != '\n') {
            putchar(fb, pitch, bpp, *x, *y, *str, color);  // Draw character
            *x += 8;  // Advance position
        }

        // Reset to top of screen if exceeding height
        if (*y + 16 >= screen_height) *y = 0;

        str++;
    }

    // After string, move to next line
    *x = 0; *y += 16;
}
