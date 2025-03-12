#include <shell.h>
#include <keyboard.h>
#include <print.h>
#include <string.h>
#include <memory.h>
#include <vfs.h>
#include <kernel.h>

#define MAX_COMMANDS 32
#define MAX_ARGS 16

static char line_buffer[SHELL_BUFFER_SIZE];

static const char *PROMPT = "arikoto> ";

char *shell_readline(const char *prompt) {
    puts(prompt, COLOR_WHITE);

    memset(line_buffer, 0, SHELL_BUFFER_SIZE);
    int pos = 0;

    while (1) {
        char c = keyboard_read();

        if (c == '\n') {
            line_buffer[pos] = '\0';
            return line_buffer;
        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                putchar('\b', COLOR_WHITE);
                putchar(' ', COLOR_WHITE);
                putchar('\b', COLOR_WHITE);
            }
        } else if (c >= ' ' && c <= '~') {
            if (pos < SHELL_BUFFER_SIZE - 1) {
                line_buffer[pos++] = c;
                putchar(c, COLOR_WHITE);
            }
        }
    }
}


typedef struct {
    char *name;
    int (*func)(int argc, char **argv);
} command_t;

static command_t commands[MAX_COMMANDS];
static int command_count = 0;

static int cmd_help(int argc, char **argv);
static int cmd_echo(int argc, char **argv);
static int cmd_clear(int argc, char **argv);
static int cmd_ls(int argc, char **argv);
static int cmd_cat(int argc, char **argv);
static int cmd_mem(int argc, char **argv);
static int cmd_reboot(int argc, char **argv);

void shell_init() {
    shell_register_command("help", cmd_help);
    shell_register_command("echo", cmd_echo);
    shell_register_command("clear", cmd_clear);
    shell_register_command("ls", cmd_ls);
    shell_register_command("cat", cmd_cat);
    shell_register_command("mem", cmd_mem);
    shell_register_command("reboot", cmd_reboot);
}

static int parse_args(char *input, char **argv) {
    int argc = 0;
    char *token;
    char *rest = input;

    while ((token = strtok_r(rest, " \t", &rest)) != NULL && argc < MAX_ARGS) {
        argv[argc++] = token;
    }

    argv[argc] = NULL;
    return argc;
}

void shell_run() {
    puts("Arikoto Shell started. Type commands below:", COLOR_WHITE);

    while (1) {
        char *input = shell_readline(PROMPT);

        if (strcmp(input, "exit") == 0) {
            puts("Shell exiting...", COLOR_YELLOW);
            return;
        }

        if (strlen(input) > 0) {
            shell_execute(input);
        }
    }
}

int shell_execute(const char *command) {
    char cmd_buffer[SHELL_BUFFER_SIZE];
    strcpy(cmd_buffer, command);

    char *argv[MAX_ARGS];
    int argc = parse_args(cmd_buffer, argv);

    if (argc == 0) {
        return 0;
    }

    for (int i = 0; i < command_count; i++) {
        if (strcmp(commands[i].name, argv[0]) == 0) {
            return commands[i].func(argc, argv);
        }
    }

    puts("Unknown command:", COLOR_WHITE);
    puts(argv[0], COLOR_WHITE);
    return -1;
}

void shell_register_command(const char *name, int (*func)(int argc, char **argv)) {
    if (command_count < MAX_COMMANDS) {
        commands[command_count].name = (char *)name;
        commands[command_count].func = func;
        command_count++;
    }
}

static int cmd_help() {
    puts("Available commands:", COLOR_WHITE);

    for (int i = 0; i < command_count; i++) {
        puts(commands[i].name, COLOR_GREEN);
    }

    return 0;
}

static int cmd_echo(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        puts(argv[i], COLOR_WHITE);
    }
    return 0;
}

static int cmd_clear() {
    screen_clear();
    return 0;
}

static int cmd_ls() {
    char buffer[1024];
    buffer[0] = '\0';

    int result = vfs_list_files(buffer, sizeof(buffer));

    if (result >= 0) {
        char *file = buffer;
        while (*file) {
            puts(file, COLOR_CYAN);

            file += strlen(file) + 1;

            if (file >= buffer + sizeof(buffer)) {
                break;
            }
        }
    } else {
        puts("Failed to list files", COLOR_RED);
    }

    return 0;
}

static int cmd_cat(int argc, char **argv) {
    if (argc < 2) {
        puts("Usage: cat <filename>", COLOR_YELLOW);
        return -1;
    }

    char buffer[1024];

    if (vfs_read(argv[1], buffer, sizeof(buffer)) >= 0) {
        puts(buffer, COLOR_WHITE);
    } else {
        puts("Failed to read file: ", COLOR_RED);
        puts(argv[1], COLOR_RED);
        return -1;
    }

    return 0;
}

static int cmd_mem() {
    char buf[64];

    puts("Memory Information:", COLOR_WHITE);

    size_t total = get_total_memory() / 1024;
    size_t used = get_used_memory() / 1024;
    size_t free = get_free_memory() / 1024;

    puts("  Total: ", COLOR_WHITE);
    itoa(total, buf, 10);
    puts(buf, COLOR_GREEN);
    puts(" KB", COLOR_WHITE);

    puts("  Used:  ", COLOR_WHITE);
    itoa(used, buf, 10);
    puts(buf, COLOR_YELLOW);
    puts(" KB (", COLOR_WHITE);
    itoa((used * 100) / total, buf, 10);
    puts(buf, COLOR_YELLOW);
    puts("%)", COLOR_WHITE);

    puts("  Free:  ", COLOR_WHITE);
    itoa(free, buf, 10);
    puts(buf, COLOR_GREEN);
    puts(" KB (", COLOR_WHITE);
    itoa((free * 100) / total, buf, 10);
    puts(buf, COLOR_GREEN);
    puts("%)", COLOR_WHITE);

    return 0;
}

static int cmd_reboot() {
    puts("Rebooting system...", COLOR_RED);

    // Send reboot command to PS/2 controller
    outb(0x64, 0xFE);

    // Wait for reboot...
    for (volatile int i = 0; i < 10000000; i++) {
    }

    // Okay, now this is bad
    panic("PANIC: Arikoto failed to reboot, hold down the power button.");

    /* This should not execute because we panicked, but
    I'm including it because you know how compilers be. */
    return -1;
}
