#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "framebuffer.h"   // Include framebuffer management
#include "memory.h"        // Include memory management
#include "scheduler.h"     // Include task scheduler

// Halt function
static void hcf(void) {
    for (;;) {
#if defined (__x86_64__)
        asm ("hlt");
#endif
    }
}

// Kernel main function
void kmain(void) {
    // Initialize the framebuffer
    framebuffer_init();

    // Initialize memory management
    memory_init();

    // Initialize task scheduler
    scheduler_init();

    // Log "HELLO!" to the framebuffer at coordinates (10, 10)
    log_to_framebuffer(10, 10, "HELLO!");

    // Hang forever
    hcf();
}
