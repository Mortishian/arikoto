#include "memory.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <request.h>
#include <kernel.h>
#include <print.h>
#include <memory.h>

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
    struct limine_framebuffer *framebuffer = framebuffer_info.response->framebuffers[0];
    uint32_t width = framebuffer_info.response->framebuffers[0]->width;
    uint32_t height = framebuffer_info.response->framebuffers[0]->height;
    uint32_t bpp_info = framebuffer_info.response->framebuffers[0]->bpp;
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

void display_memory_info(uint32_t *fb, size_t pitch, size_t bpp, size_t *x, size_t *y) {
    // Get framebuffer information
    struct limine_framebuffer *framebuffer = framebuffer_info.response->framebuffers[0];
    size_t max_width = framebuffer->width;
    size_t max_height = framebuffer->height;

    // Display total memory information
    char buffer[64];
    itoa(get_total_memory() / 1024, buffer, 10);
    char total_memory_info[128];
    int i = 0;
    const char* total_label = "Total memory: ";
    while (total_label[i] != '\0') {
        total_memory_info[i] = total_label[i];
        i++;
    }
    int j = 0;
    while (buffer[j] != '\0') {
        total_memory_info[i] = buffer[j];
        i++;
        j++;
    }
    total_memory_info[i++] = ' ';
    total_memory_info[i++] = 'K';
    total_memory_info[i++] = 'B';
    total_memory_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, total_memory_info, COLOR_WHITE, max_width, max_height);

    // Display used memory information
    itoa(get_used_memory() / 1024, buffer, 10);
    char used_memory_info[128];
    i = 0;
    const char* used_label = "Used memory: ";
    while (used_label[i] != '\0') {
        used_memory_info[i] = used_label[i];
        i++;
    }
    int k = 0;
    while (buffer[k] != '\0') {
        used_memory_info[i] = buffer[k];
        i++;
        k++;
    }
    used_memory_info[i++] = ' ';
    used_memory_info[i++] = 'K';
    used_memory_info[i++] = 'B';
    used_memory_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, used_memory_info, COLOR_WHITE, max_width, max_height);

    // Display free memory information
    itoa(get_free_memory() / 1024, buffer, 10);
    char free_memory_info[128];
    i = 0;
    const char* free_label = "Free memory: ";
    while (free_label[i] != '\0') {
        free_memory_info[i] = free_label[i];
        i++;
    }
    int l = 0;
    while (buffer[l] != '\0') {
        free_memory_info[i] = buffer[l];
        i++;
        l++;
    }
    free_memory_info[i++] = ' ';
    free_memory_info[i++] = 'K';
    free_memory_info[i++] = 'B';
    free_memory_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, free_memory_info, COLOR_WHITE, max_width, max_height);

    // Allocate and free a page
    void *page1 = allocate_page();
    itoa((uintptr_t)page1, buffer, 16);
    char page_info[128];
    i = 0;
    const char* alloc_label = "Allocated page at: 0x";
    while (alloc_label[i] != '\0') {
        page_info[i] = alloc_label[i];
        i++;
    }
    int m = 0;
    while (buffer[m] != '\0') {
        page_info[i] = buffer[m];
        i++;
        m++;
    }
    page_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, page_info, COLOR_GREEN, max_width, max_height);

    free_page(page1);
    char freed_page_info[128];
    i = 0;
    const char* freed_label = "Freed page at: 0x";
    while (freed_label[i] != '\0') {
        freed_page_info[i] = freed_label[i];
        i++;
    }
    int n = 0;
    while (buffer[n] != '\0') {
        freed_page_info[i] = buffer[n];
        i++;
        n++;
    }
    freed_page_info[i] = '\0';
    puts(fb, pitch, bpp, x, y, freed_page_info, COLOR_YELLOW, max_width, max_height);
}


// Kernel main function.
void kmain(void) {
    // Start PMM
    init_pmm();

    // Ensure we have a framebuffer.
    if (framebuffer_info.response == NULL || framebuffer_info.response->framebuffer_count < 1) {
        hcf();
    }

    struct limine_framebuffer *framebuffer = framebuffer_info.response->framebuffers[0];
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

    display_memory_info(fb, pitch, bpp, &x, &y);

    // We're done, just hang...
    hcf();
}
