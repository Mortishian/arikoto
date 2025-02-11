#pragma once

#include <stddef.h>

struct vfs_operations {
    int (*read)(const char *path, char *buffer, size_t size);
    int (*delete)(const char *path);
    int (*create)(const char *path, const char *data);
};

int vfs_mount(const char *path, struct vfs_operations *ops);

int vfs_list_files(char *buffer, size_t size);

int vfs_create(const char *name, const char *data);

int vfs_read(const char *path, char *buffer, size_t size);

int vfs_delete(const char *path);

int ramdisk_create(const char *name, const char *data);

int ramdisk_read(const char *path, char *buffer, size_t size);

int ramdisk_delete(const char *path);

int ramdisk_list(char *buffer, size_t size);

extern struct vfs_operations ramdisk_ops;

extern char buffer[1024];
