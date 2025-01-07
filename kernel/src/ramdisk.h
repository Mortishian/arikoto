#ifndef RAMDISK_H
#define RAMDISK_H

int ramdisk_create(const char *name, const char *data);

int ramdisk_read(const char *path, char *buffer, size_t size);

extern int file_count;

#endif
