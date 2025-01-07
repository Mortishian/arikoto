#ifndef VFS_H
#define VFS_H

#include <stddef.h>

// VFS Operations structure
struct vfs_operations {
    int (*read)(const char *path, char *buffer, size_t size);
};

// VFS functions
int vfs_mount(const char *path, struct vfs_operations *ops);
int vfs_read(const char *path, char *buffer, size_t size);

extern char buffer[128];


#endif
