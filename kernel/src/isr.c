#include <kernel.h>

void isr0() {
    panic("PANIC: Division by zero exception!\n");
}
