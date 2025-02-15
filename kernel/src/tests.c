#include <kernel.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <request.h>
#include <print.h>
#include <vfs.h>

static void display_framebuffer_info() {
    // Get framebuffer information
    uint32_t width = framebuffer_info.response->framebuffers[0]->width;
    uint32_t height = framebuffer_info.response->framebuffers[0]->height;
    uint32_t bpp_info = framebuffer_info.response->framebuffers[0]->bpp;

    puts("Framebuffer Info:", COLOR_WHITE); // Title

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
    puts(width_info, COLOR_WHITE);

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
    puts(height_info, COLOR_WHITE);

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
    puts(bpp_info_str, COLOR_WHITE);
}

static void display_memory_info() {
    // Display total memory information
    puts("Memory Info:", COLOR_WHITE); // Title

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
    puts(total_memory_info, COLOR_WHITE);

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
    puts(used_memory_info, COLOR_WHITE);

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
    puts(free_memory_info, COLOR_WHITE);

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
    puts(page_info, COLOR_GREEN);

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
    puts(freed_page_info, COLOR_YELLOW);
}

void vfs_test(void) {
    vfs_mount("/", &ramdisk_ops);
    vfs_create("/roadmap.txt", "Where do we go from here?\n\nList of components working, minimally implemented or still to come:\nFramebuffer: Implemented\nVFS: Minimal implementation\nScheduler: Minimal implementation\nMemory: Minimal implementation\nInput: Still to come\nShell: Still to come\nlibc:Still to come\nTools: Still to come\nGUI: Stil to come");
    vfs_read("/roadmap.txt", buffer, sizeof(buffer));
    vfs_delete("/roadmap.txt");
}

void display_info(void) {
    display_framebuffer_info();
    display_memory_info();
}
