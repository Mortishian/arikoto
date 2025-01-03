#ifndef KERNEL_H
#define KERNEL_H

#include <limine.h>

static volatile struct limine_memmap_request memorymap_info;
static volatile struct limine_framebuffer_request framebuffer_info;

// Halt and catch fire function (obvious).
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

#endif
