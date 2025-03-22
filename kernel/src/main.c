#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <request.h>
#include <kernel.h>
#include <print.h>
#include <memory.h>
#include <vfs.h>
#include <scheduler.h>
#include <keyboard.h>
#include <shell.h>
#include <gdt.h>

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

    puts("[Arikoto 0.0.2]", COLOR_RED);
    puts("   _|_|              _|  _|                    _|                ", COLOR_BLUE);
    puts(" _|    _|  _|  _|_|      _|  _|      _|_|    _|_|_|_|    _|_|    ", COLOR_CYAN);
    puts(" _|_|_|_|  _|_|      _|  _|_|      _|    _|    _|      _|    _|  ", COLOR_GREEN);
    puts(" _|    _|  _|        _|  _|  _|    _|    _|    _|      _|    _|  ", COLOR_MAGENTA);
    puts(" _|    _|  _|        _|  _|    _|    _|_|        _|_|    _|_|   \n ", COLOR_YELLOW);
    puts("Check out arikoto.nerdnextdoor.net!", COLOR_WHITE);

    /* Start GDT */
    init_gdt();

    /* Start PMM */
    init_pmm();

    /* Start scheduler */
    scheduler_t scheduler;
    scheduler_init(&scheduler, 2); /* Set the time quantum to 2 (arbitrary units) */

    /*
    * unused: scheduler_add_process(&scheduler, 1, display_info);
    */
    scheduler_add_process(&scheduler, 1, vfs_test);
    scheduler_add_process(&scheduler, 2, shell_init);
    scheduler_add_process(&scheduler, 3, shell_run);

    scheduler_run(&scheduler);
}
