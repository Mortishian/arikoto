#include <stddef.h>
#include <string.h>

struct vfs_operations {
    int (*read)(const char *path, char *buffer, size_t size);
};

#define MAX_MOUNT_POINTS 10
struct mount_point {
    char path[64];
    struct vfs_operations *ops;
} mount_table[MAX_MOUNT_POINTS];

size_t mount_count = 0;

int vfs_mount(const char *path, struct vfs_operations *ops) {
    if (mount_count >= MAX_MOUNT_POINTS) return -1;

    strcpy(mount_table[mount_count].path, path);
    mount_table[mount_count].ops = ops;
    mount_count++;
    return 0;
}

int vfs_read(const char *path, char *buffer, size_t size) {
    for (size_t i = 0; i < mount_count; i++) {
        if (strncmp(path, mount_table[i].path, strlen(mount_table[i].path)) == 0) {
            return mount_table[i].ops->read(path + strlen(mount_table[i].path), buffer, size);
        }
    }
    return -1;
}

char buffer[128];
