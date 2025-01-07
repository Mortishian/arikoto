#include <stddef.h>

// String copy
char *strcpy(char *dest, const char *src) {
    char *original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

// String compare
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// String length
size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++) len++;
    return len;
}

// Memory copy
void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0') {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}
