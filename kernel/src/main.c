#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <print.h>
#include <kernel.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request limine_framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Helper function to convert integer to string
void itoa(int num, char *str, int base) {
    int i = 0;
    bool isNegative = false;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // Append negative sign for negative numbers
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void display_system_info(uint32_t *fb, size_t pitch, size_t bpp, size_t *x, size_t *y) {
    // Get framebuffer information
    struct limine_framebuffer *framebuffer = limine_framebuffer_request.response->framebuffers[0];
    uint32_t width = limine_framebuffer_request.response->framebuffers[0]->width;
    uint32_t height = limine_framebuffer_request.response->framebuffers[0]->height;
    uint32_t bpp_info = limine_framebuffer_request.response->framebuffers[0]->bpp;
    size_t max_width = framebuffer->width;
    size_t max_height = framebuffer->height;

    puts(fb, pitch, bpp, x, y, "Framebuffer Info:", COLOR_WHITE, max_width, max_height); // Title

    char buffer[64];

    itoa(width, buffer, 10);
    char width_info[128];
    int i = 0;

    const char* width_label = "Width: ";
    while (width_label[i] != '\0') {
        width_info[i] = width_label[i];
        i++;
    }

    int j = 0;
    while (buffer[j] != '\0') {
        width_info[i] = buffer[j];
        i++;
        j++;
    }

    width_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, width_info, COLOR_WHITE, max_width, max_height);

    itoa(height, buffer, 10);
    char height_info[128];
    i = 0;

    const char* height_label = "Height: ";
    while (height_label[i] != '\0') {
        height_info[i] = height_label[i];
        i++;
    }

    int k = 0;
    while (buffer[k] != '\0') {
        height_info[i] = buffer[k];
        i++;
        k++;
    }

    height_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, height_info, COLOR_WHITE, max_width, max_height);

    itoa(bpp_info, buffer, 10);
    char bpp_info_str[128];
    i = 0;

    const char* bpp_label = "BPP: ";
    while (bpp_label[i] != '\0') {
        bpp_info_str[i] = bpp_label[i];
        i++;
    }

    int l = 0;
    while (buffer[l] != '\0') {
        bpp_info_str[i] = buffer[l];
        i++;
        l++;
    }

    bpp_info_str[i] = '\0';
    puts(fb, pitch, bpp, x, y, bpp_info_str, COLOR_WHITE, max_width, max_height);
}

// Kernel main function.
void kmain(void) {
    // Ensure we have a framebuffer.
    if (limine_framebuffer_request.response == NULL || limine_framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    struct limine_framebuffer *framebuffer = limine_framebuffer_request.response->framebuffers[0];
    uint32_t *fb = framebuffer->address;
    size_t pitch = framebuffer->pitch;
    size_t bpp = framebuffer->bpp;

    // Clear screen
    for (size_t i = 0; i < framebuffer->height * framebuffer->width; i++) {
        fb[i] = COLOR_BLACK;
    }

    // Initial cursor position
    size_t x = 0;
    size_t y = 0;
    size_t max_width = framebuffer->width;
    size_t max_height = framebuffer->height;

    puts(fb, pitch, bpp, &x, &y, "[Arikoto 0.0.1]", COLOR_RED, max_width, max_height);

    display_system_info(fb, pitch, bpp, &x, &y);

    // We're done, just hang...
    hcf();
}
