#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <vfs.h>
#include <print.h>

#define MAX_MOUNT_POINTS 10
#define MAX_FILES 10
#define MAX_FILE_SIZE 256

char buffer[1024];

// RAM Disk Operations
struct vfs_operations ramdisk_ops = {
    .read = ramdisk_read,
    .delete = ramdisk_delete,
    .create = ramdisk_create,
};

// File Structure
struct vfs_file {
    char name[64];
    char data[MAX_FILE_SIZE];
    size_t size;
};

// File Table
struct vfs_file file_table[MAX_FILES];
int file_count = 0;

// Mount Point Structure
struct mount_point {
    char path[64];
    struct vfs_operations *ops;
} mount_table[MAX_MOUNT_POINTS];

size_t mount_count = 0;

int ramdisk_create(const char *name, const char *data) {
    if (file_count >= MAX_FILES) {
        puts("Failed to create file: maximum files reached", COLOR_RED);
        return -1;
    }

    struct vfs_file *file = &file_table[file_count++];
    strcpy(file->name, name);
    strcpy(file->data, data);
    file->size = strlen(data);

    puts("File created successfully", COLOR_GREEN);
    return 0;
}

int ramdisk_read(const char *path, char *buffer, size_t size) {
    for (size_t i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].name, path) == 0) {
            size_t to_copy = file_table[i].size < size ? file_table[i].size : size;
            memcpy(buffer, file_table[i].data, to_copy);
            puts(buffer, COLOR_WHITE);
            return 0;
        }
    }
    puts("Failed to read file", COLOR_RED);
    return -1;
}

int ramdisk_delete(const char *path) {
    for (size_t i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].name, path) == 0) {
            for (size_t j = i; j < file_count - 1; j++) {
                file_table[j] = file_table[j + 1];
            }
            file_count--;
            puts("File deleted successfully", COLOR_GREEN);
            return 0;
        }
    }
    puts("Failed to delete file", COLOR_RED);
    return -1;
}

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

int vfs_delete(const char *path) {
    for (size_t i = 0; i < mount_count; i++) {
        if (strncmp(path, mount_table[i].path, strlen(mount_table[i].path)) == 0) {
            return mount_table[i].ops->delete(path + strlen(mount_table[i].path));
        }
    }
    return -1;
}

int vfs_create(const char *path, const char *data) {
    for (size_t i = 0; i < mount_count; i++) {
        if (strncmp(path, mount_table[i].path, strlen(mount_table[i].path)) == 0) {
            if (mount_table[i].ops->create) {
                return mount_table[i].ops->create(path + strlen(mount_table[i].path), data);
            } else {
                return -1;
            }
        }
    }
    return -1;
}
