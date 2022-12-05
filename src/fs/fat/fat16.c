#include "fat16.h"
#include "status.h"
#include "memory/memory.h"
#include <stddef.h>

int fat16_resolve(struct disk* disk);
void* fat_open(struct disk* disk, struct path_part* path, FILE_MODE mode);

struct filesystem fat16_fs = {
    .resolve = fat16_resolve,
    .open = fat_open
};

struct filesystem* fat16_init(){
    const char* name = "FAT16";
    strncpy(fat16_fs.name, name, sizeof(name));
    return &fat16_fs;
}

int fat16_resolve(struct disk* disk){
    //Returning 0 signifies that the filesystem has been resolved
    return 0;
}

void* fat_open(struct disk* disk, struct path_part* path, FILE_MODE mode){
    return NULL;
}