#pragma once

#include <stddef.h>

/* Page size definition */
#define PAGE_SIZE 4096

/* Initialize the physical memory manager */
void init_pmm(void);

/* Allocate a single page of memory */
void *allocate_page(void);

/* Free a previously allocated page of memory */
void free_page(void *page);

/* Get total memory size in bytes */
size_t get_total_memory(void);

/* Get used memory size in bytes */
size_t get_used_memory(void);

/* Get free memory size in bytes */
size_t get_free_memory(void);
