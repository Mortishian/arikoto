#include <keyboard.h>
#include <print.h>
#include <stdint.h>
#include <stdbool.h>

#define PS2_DATA_PORT    0x60
#define PS2_STATUS_PORT  0x64
#define PS2_COMMAND_PORT 0x64

static bool shift_pressed = false;
static bool caps_lock = false;
static bool ctrl_pressed = false;
static bool alt_pressed = false;
static bool extended_key = false;

static char key_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_start = 0;
static int buffer_end = 0;

static const char scan_code_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static const char scan_code_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static void ps2_wait_for_controller() {
    while ((inb(PS2_STATUS_PORT) & 2) != 0) {
    }
}

static void keyboard_buffer_add(char c) {
    if ((buffer_end + 1) % KEYBOARD_BUFFER_SIZE != buffer_start) {
        key_buffer[buffer_end] = c;
        buffer_end = (buffer_end + 1) % KEYBOARD_BUFFER_SIZE;
    }
}

void init_keyboard() {
    ps2_wait_for_controller();
    outb(PS2_COMMAND_PORT, 0xAE);

    inb(PS2_DATA_PORT);

    ps2_wait_for_controller();
    outb(PS2_DATA_PORT, 0xFF);

    shift_pressed = false;
    caps_lock = false;
    ctrl_pressed = false;
    alt_pressed = false;
    extended_key = false;
    buffer_start = 0;
    buffer_end = 0;

    puts("Keyboard initialized", COLOR_GREEN);
}

void keyboard_callback() {
    uint8_t scan_code = inb(PS2_DATA_PORT);

    if (scan_code == KEY_EXTENDED) {
        extended_key = true;
        return;
    }

    if (scan_code & KEY_RELEASE) {
        scan_code &= ~KEY_RELEASE;

        if (scan_code == KEY_LSHIFT || scan_code == KEY_RSHIFT) {
            shift_pressed = false;
        } else if (scan_code == KEY_LCTRL) {
            ctrl_pressed = false;
        } else if (scan_code == KEY_LALT) {
            alt_pressed = false;
        }

        extended_key = false;
        return;
    }

    if (extended_key) {
        extended_key = false;
        return;
    }

    if (scan_code == KEY_LSHIFT || scan_code == KEY_RSHIFT) {
        shift_pressed = true;
        return;
    } else if (scan_code == KEY_LCTRL) {
        ctrl_pressed = true;
        return;
    } else if (scan_code == KEY_LALT) {
        alt_pressed = true;
        return;
    } else if (scan_code == KEY_CAPSLOCK) {
        caps_lock = !caps_lock;
        return;
    }

    if (scan_code < sizeof(scan_code_ascii)) {
        char ascii;

        if (shift_pressed) {
            ascii = scan_code_shift[scan_code];
        } else {
            ascii = scan_code_ascii[scan_code];
        }

        if (caps_lock && ascii >= 'a' && ascii <= 'z') {
            ascii = ascii - 'a' + 'A';
        } else if (caps_lock && ascii >= 'A' && ascii <= 'Z') {
            ascii = ascii - 'A' + 'a';
        }

        if (ctrl_pressed && ascii >= 'a' && ascii <= 'z') {
            ascii = ascii - 'a' + 1;
        }

        if (ascii) {
            keyboard_buffer_add(ascii);
        }
    }
}

char keyboard_read() {
    while (buffer_start == buffer_end) {
        uint8_t status = inb(PS2_STATUS_PORT);

        if (status & 1) {
            keyboard_callback();

            // If we got something, break
            if (buffer_start != buffer_end) {
                break;
            }
        }

        for (volatile int i = 0; i < 10000; i++) {}
    }

    char c = key_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

bool keyboard_has_key() {
    keyboard_callback();
    return buffer_start != buffer_end;
}

char* keyboard_get_buffer() {
    return key_buffer;
}

void keyboard_clear_buffer() {
    buffer_start = 0;
    buffer_end = 0;
}
