#ifndef STRING_H
#define STRING_H

char *strcpy(char *dest, const char *src);

int strcmp(const char *s1, const char *s2);

size_t strlen(const char *str);

void *memcpy(void *dest, const void *src, size_t n);

int strncmp(const char *s1, const char *s2, size_t n);

#endif
