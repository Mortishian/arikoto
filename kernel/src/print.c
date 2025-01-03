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

    size_t bytes_per_line = font->charsize / 8;
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
void puts(uint32_t *fb, size_t pitch, size_t bpp, size_t *x, size_t *y, const char *str, uint32_t color, size_t max_width, size_t screen_height) {
    // Keep track of the initial X position for each puts call
    size_t start_x = *x;

    while (*str) {
        if (*str == '\n') {
            // Move to the next line when encountering a newline character
            *x = start_x;  // Reset x to the start of the line
            *y += 16; // Move down by the height of a character (16 pixels)
        } else {
            // Otherwise, render the character
            putchar(fb, pitch, bpp, *x, *y, *str++, color);
            *x += 8; // Advance to the next character position (8 pixels wide)
        }

        // Check if the text exceeds the screen width
        if (*x >= max_width) {
            *x = start_x;  // Reset x to the start of the line
            *y += 16; // Move down by the height of a character
        }

        // If we exceed the screen height, reset to the top to overwrite
        if (*y >= screen_height) {
            *y = 0; // Reset the y-coordinate to the top (0)
        }
    }

    // After the string is rendered, go to the next line automatically
    *x = start_x;  // Reset x to the start of the line
    *y += 16; // Move down by the height of a character (16 pixels)
}
