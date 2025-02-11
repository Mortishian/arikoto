#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <request.h>
#include <kernel.h>
#include <print.h>
#include <memory.h>
#include <vfs.h>

// Kernel main function.
void kmain(void) {

    // Start PMM
    init_pmm();

    // Ensure we have a framebuffer.
    if (framebuffer_info.response == NULL || framebuffer_info.response->framebuffer_count < 1) {
        hcf();
    }

    // Extract framebuffer information
    struct limine_framebuffer *framebuffer = framebuffer_info.response->framebuffers[0];
    uint32_t *fb = framebuffer->address;
    size_t pitch = framebuffer->pitch;
    size_t bpp = framebuffer->bpp;
    size_t max_width = framebuffer->width;
    size_t max_height = framebuffer->height;

    // Clear the screen
    for (size_t i = 0; i < framebuffer->height * framebuffer->width; i++) {
        fb[i] = COLOR_BLACK;
    }

    // Start framebuffer
    init_framebuffer(fb, pitch, bpp, max_width, max_height);

    puts("[Arikoto 0.0.2]", COLOR_RED);

    display_framebuffer_info();

    display_memory_info();

    vfs_mount("/", &ramdisk_ops);

    if (vfs_create("/hello.txt", "Welcome to Arikoto!!!") == 0) {
        puts("File created successfully", COLOR_GREEN);
    } else {
        puts("Failed to create file", COLOR_RED);
    }

    if (vfs_read("/hello.txt", buffer, sizeof(buffer)) == 0) {
        puts(buffer, COLOR_WHITE);
    } else {
        puts("Failed to read file", COLOR_RED);
    }

    if (vfs_delete("/hello.txt") == 0) {
        puts("File deleted successfully", COLOR_GREEN);
    } else {
        puts("Failed to delete file", COLOR_RED);
    }

    if (vfs_read("/hello.txt", buffer, sizeof(buffer)) == 0) {
        puts(buffer, COLOR_WHITE);
    } else {
        puts("Failed to read file", COLOR_RED);
    }

    puts("The quick brown fox jumps over the lazy dog. 0123456789The quick brown fox jumps over the lazy dog. 0123456789The quick brown fox jumps over the lazy dog. 0123456789", COLOR_WHITE);

    // We're done, just hang...
    hcf();

}
