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

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0') {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}

size_t strspn(const char *str, const char *accept) {
    const char *p;
    const char *a;
    size_t count = 0;

    for (p = str; *p != '\0'; ++p) {
        for (a = accept; *a != '\0'; ++a) {
            if (*p == *a) {
                break;
            }
        }
        if (*a == '\0') {
            return count;
        }
        ++count;
    }

    return count;
}

char *strpbrk(const char *str, const char *accept) {
    const char *a;

    while (*str != '\0') {
        for (a = accept; *a != '\0'; ++a) {
            if (*str == *a) {
                return (char *)str;
            }
        }
        ++str;
    }

    return NULL;
}

char *strchr(const char *str, int c) {
    while (*str != '\0') {
        if (*str == c) {
            return (char *)str;
        }
        ++str;
    }

    if (c == '\0') {
        return (char *)str;
    }

    return NULL;
}

char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *token_start;

    if (str == NULL) {
        str = *saveptr;
    }

    while (*str && strchr(delim, *str)) {
        str++;
    }

    if (*str == '\0') {
        *saveptr = str;
        return NULL;
    }

    token_start = str;

    while (*str && !strchr(delim, *str)) {
        str++;
    }

    if (*str) {
        *str = '\0';
        *saveptr = str + 1;
    } else {
        *saveptr = str;
    }

    return token_start;
}
