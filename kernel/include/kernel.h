#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Halt and catch fire function (obvious).
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

void display_info(void);
void vfs_test(void);

// Helper function to convert integer to string
static inline char* itoa(int num, char *str, int base) {
    int i = 0;
    bool isNegative = false;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // Append negative sign for negative numbers
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    // Just to shut the compiler up
    return 0;
}
