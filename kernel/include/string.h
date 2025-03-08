#pragma once

char *strcpy(char *dest, const char *src);

int strcmp(const char *s1, const char *s2);

size_t strlen(const char *str);

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *s, int c, size_t n);

int strncmp(const char *s1, const char *s2, size_t n);

char *strtok_r(char *str, const char *delim, char **saveptr);

size_t strspn(const char *str, const char *accept);

char *strchr(const char *str, int c);
