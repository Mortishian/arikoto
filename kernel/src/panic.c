#include <kernel.h>
#include <print.h>
#include <stdarg.h>

// This is only for panic, won't be used anywhere else... yet.
static void panic_vfprintf(const char *fmt, va_list args) {
    char buffer[256];
    size_t pos = 0;

    while (*fmt && pos < sizeof(buffer) - 1) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    int num = va_arg(args, int);
                    char num_buf[32];
                    itoa(num, num_buf, 10);
                    size_t num_len = strlen(num_buf);
                     if (pos + num_len < sizeof(buffer)) {
                        strcpy(buffer + pos, num_buf);
                        pos += num_len;
                    }
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    char num_buf[32];
                    itoa(num, num_buf, 16);
                    size_t num_len = strlen(num_buf);
                     if (pos + num_len < sizeof(buffer)) {
                        strcpy(buffer + pos, num_buf);
                        pos += num_len;
                    }
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char *);
                    size_t str_len = strlen(str);
                    if(pos + str_len < sizeof(buffer)){
                       strcpy(buffer + pos, str);
                       pos += str_len;
                    }

                    break;
                }
                case 'c': {
                  if(pos + 1 < sizeof(buffer)) {
                      buffer[pos++] = (char)va_arg(args, int);
                  }
                    break;
                }
                case '%': {
                    if (pos + 1 < sizeof(buffer)) {
                         buffer[pos++] = '%';
                    }

                    break;
                }
                default:
                    break;
            }
        } else {
            buffer[pos++] = *fmt;
        }
        fmt++;
    }

      if (pos < sizeof(buffer)) {
        buffer[pos] = '\0';
    }

    puts(buffer, COLOR_WHITE);
}

void panic_dump_registers(void) {
    asm volatile (
        "push %%rbp\n"
        "push %%r15\n"
        "push %%r14\n"
        "push %%r13\n"
        "push %%r12\n"
        "push %%r11\n"
        "push %%r10\n"
        "push %%r9\n"
        "push %%r8\n"
        "push %%rdi\n"
        "push %%rsi\n"
        "push %%rdx\n"
        "push %%rcx\n"
        "push %%rbx\n"
        "push %%rax\n"
        "pushfq\n"

        "mov %%rsp, %%rdi\n"
        "call panic_print_registers\n"

        "popfq\n"
        "pop %%rax\n"
        "pop %%rbx\n"
        "pop %%rcx\n"
        "pop %%rdx\n"
        "pop %%rsi\n"
        "pop %%rdi\n"
        "pop %%r8\n"
        "pop %%r9\n"
        "pop %%r10\n"
        "pop %%r11\n"
        "pop %%r12\n"
        "pop %%r13\n"
        "pop %%r14\n"
        "pop %%r15\n"
        "pop %%rbp\n"
        :
        :
        : "memory"
    );
}

extern void panic_dump_registers(void);

void panic_print_registers_formatted(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    panic_vfprintf(fmt, args);
    va_end(args);
}

void panic_print_registers(uint64_t *rsp) {
    puts("Registers:", COLOR_WHITE);
    panic_print_registers_formatted("  RFLAGS: 0x%x", rsp[0]);
    panic_print_registers_formatted("  RAX: 0x%x", rsp[1]);
    panic_print_registers_formatted("  RBX: 0x%x", rsp[2]);
    panic_print_registers_formatted("  RCX: 0x%x", rsp[3]);
    panic_print_registers_formatted("  RDX: 0x%x", rsp[4]);
    panic_print_registers_formatted("  RSI: 0x%x", rsp[5]);
    panic_print_registers_formatted("  RDI: 0x%x", rsp[6]);
    panic_print_registers_formatted("  R8:  0x%x", rsp[7]);
    panic_print_registers_formatted("  R9:  0x%x", rsp[8]);
    panic_print_registers_formatted("  R10: 0x%x", rsp[9]);
    panic_print_registers_formatted("  R11: 0x%x", rsp[10]);
    panic_print_registers_formatted("  R12: 0x%x", rsp[11]);
    panic_print_registers_formatted("  R13: 0x%x", rsp[12]);
    panic_print_registers_formatted("  R14: 0x%x", rsp[13]);
    panic_print_registers_formatted("  R15: 0x%x", rsp[14]);
    panic_print_registers_formatted("  RBP: 0x%x", rsp[15]);
}

void panic(const char *fmt, ...) {
    asm volatile ("cli");
    screen_clear();
    puts("****************************************\n", COLOR_RED);
    puts("              KERNEL PANIC\n", COLOR_RED);
    puts("****************************************\n", COLOR_RED);


    puts("Arikoto has panicked. If this occurs frequently and you didn't cause this intentionally, please submit it on GitHub.\n\n", COLOR_WHITE);
    va_list args;
    va_start(args, fmt);
    panic_vfprintf(fmt, args);
    va_end(args);

    panic_dump_registers();

    puts("\nSystem halted.", COLOR_WHITE);

    hcf();
}
