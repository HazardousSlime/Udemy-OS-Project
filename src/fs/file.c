#include "file.h"
#include "config.h"
#include <stddef.h>
#include "test/test.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"

static struct filesystem* filesystems[PEACHOS_MAX_FILESYSTEMS];
static struct file_descriptor* file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem(){
    int i = 0;
    for(i = 0; i < PEACHOS_MAX_FILESYSTEMS; ++i){
        if(filesystems[i] == 0)
            return &filesystems[i];
    }
    return NULL;
}

void fs_insert_filesystem(struct filesystem* filesystem){
    struct filesystem** fs;
    fs = fs_get_free_filesystem();
    //[TODO] Implement kernel panic
    assert(fs);

    *fs = filesystem;
}

static void fs_static_load(){
    //fs_insert_filesystem(fat16_init());
}

static void fs_load(){
    bzero(filesystems, sizeof(filesystems));
    fs_static_load();
}

void fs_init(){
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor** desc_out){
    int res = -EMEMORY;
    for(int i = 0; i < PEACHOS_MAX_FILE_DESCRIPTORS; ++i){
        if(file_descriptors[i] == NULL){
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            //Descriptors start at 1
            desc->index = i + 1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = PEACHOS_ALL_OK;
            break;
        }
    }
    return res;
}

static struct file_descriptor* file_get_descriptor(int fd){
    return (fd <= 0 || fd >= PEACHOS_MAX_FILE_DESCRIPTORS) ? NULL : file_descriptors[fd-1];
}

struct filesystem* fs_resolve(struct disk* disk){
    struct filesystem* fs = 0;
    for(int i = 0; i < PEACHOS_MAX_FILESYSTEMS; ++i){
        if(filesystems[i] != NULL && filesystems[i]->resolve(disk) == 0){
            fs = filesystems[i];
            break;
        }
    }
    return fs;
}

int fopen(const char* filename, const char* mode){
    return -EIO;
}


