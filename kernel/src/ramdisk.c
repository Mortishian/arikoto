#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <vfs.h>


#define MAX_FILES 10
#define MAX_FILE_SIZE 256

// File Structure
struct ram_file {
    char name[64];
    char data[MAX_FILE_SIZE];
    size_t size;
};

// File Table
struct ram_file file_table[MAX_FILES];
int file_count = 0;

int ramdisk_create(const char *name, const char *data) {
    if (file_count >= MAX_FILES) return -1; // Disk full

    struct ram_file *file = &file_table[file_count++];
    strcpy(file->name, name);
    strcpy(file->data, data);
    file->size = strlen(data);

    return 0;
}

int ramdisk_read(const char *path, char *buffer, size_t size) {
    for (size_t i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].name, path) == 0) {
            size_t to_copy = file_table[i].size < size ? file_table[i].size : size;
            memcpy(buffer, file_table[i].data, to_copy);
            return 0;
        }
    }
    return -1;
}

struct vfs_operations ramdisk_ops = {
    .read = ramdisk_read,
};
