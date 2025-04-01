#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <request.h>
#include <kernel.h>
#include <print.h>
#include <memory.h>
#include <vfs.h>
#include <keyboard.h>
#include <shell.h>
#include <gdt.h>
#include <idt.h>

/* Kernel main function */
void kmain(void) {
    /* Ensure we have a framebuffer */
    if (framebuffer_info.response == NULL || framebuffer_info.response->framebuffer_count < 1) {
        hcf();
    }

    /* Extract framebuffer information */
    struct limine_framebuffer *framebuffer = framebuffer_info.response->framebuffers[0];
    uint32_t *fb = framebuffer->address;
    size_t pitch = framebuffer->pitch;
    size_t bpp = framebuffer->bpp;
    size_t max_width = framebuffer->width;
    size_t max_height = framebuffer->height;

    /* Start framebuffer */
    init_framebuffer(fb, pitch, bpp, max_width, max_height);

    printk(COLOR_RED, "[Arikoto 0.0.2]\n");
    printk(COLOR_BLUE, "   _|_|              _|  _|                    _|                \n");
    printk(COLOR_CYAN, " _|    _|  _|  _|_|      _|  _|      _|_|    _|_|_|_|    _|_|    \n");
    printk(COLOR_GREEN, " _|_|_|_|  _|_|      _|  _|_|      _|    _|    _|      _|    _|  \n");
    printk(COLOR_MAGENTA, " _|    _|  _|        _|  _|  _|    _|    _|    _|      _|    _|  \n");
    printk(COLOR_YELLOW, " _|    _|  _|        _|  _|    _|    _|_|        _|_|    _|_|   \n");
    printk(COLOR_WHITE, "Check out arikoto.nerdnextdoor.net!\n");

    /* Start GDT */
    init_gdt();

    /* Start IDT */
    init_idt();

    /* Start PMM */
    init_pmm();

    /* Mount VFS and create file via vfs_test */
    vfs_test();

    /* Start shell */
    shell_init();
    shell_run();

    /* Halt system for now */
    hcf();
}
