#ifndef KERNEL_H
#define KERNEL_H

// Halt and catch fire function (obvious).
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

#endif
