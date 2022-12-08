#include "file.h"
#include "config.h"
#include <stddef.h>
#include "test/test.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "fs/fat/fat16.h"
#include "kernel.h"

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
    fs_insert_filesystem(fat16_init());
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

FILE_MODE file_get_mode_by_string(const char* str){
    FILE_MODE mode = FILE_MODE_INVALID;
    char c = str[0];
    switch (c){
        case 'r':
            mode = FILE_MODE_READ;
            break;
        case 'w':
            mode = FILE_MODE_WRITE;
            break;
        case 'a':
            mode = FILE_MODE_APPEND;
            break;
        default:
            break;
    }
    return mode;
}

int fopen(const char* filename, const char* mode_str){
    int res = PEACHOS_ALL_OK;
    struct path_root root_path;
    res = path_parse(filename, "", &root_path);
    if(res){
        goto out;
    }
    //Must be a file, not root directory
    if(!root_path.first){
        res = -EINVARG;
        goto out;
    }
    struct disk* disk = disk_get(root_path.drive_no);
    if(!disk){
        res = -EIO;
        goto out;
    }

    if(!disk->filesystem){
        res = -EIO;
        goto out;
    }

    FILE_MODE mode = file_get_mode_by_string(mode_str);
    if(mode == FILE_MODE_INVALID){
        res = -EINVARG;
        goto out;
    }

    void* descriptor_private_data = disk->filesystem->open(disk, root_path.first, mode);
    if(ISERR(descriptor_private_data)){
        res = ERROR_I(descriptor_private_data);
        goto out;
    }

    struct file_descriptor* desc = NULL;
    res = file_new_descriptor(&desc);
    if(res < 0){
        goto out;
    }
    desc->filesystem = disk->filesystem;
    desc->private = descriptor_private_data;
    desc->disk = disk;
    res = desc->index;
out:
    res = (res < 0) ? 0 : res;
    return res;
}

int fread(void* ptr, uint32_t size, uint32_t nmemb, int fd){
    int res = 0;
    if(size == 0 || nmemb == 0 || fd < 1){
        res = -EINVARG;
        goto out;
    }
    struct file_descriptor* desc = file_get_descriptor(fd);
    if(!desc){
        res = -EIO;
        goto out;
    }
    res = desc->filesystem->read(desc->disk, desc->private, size, nmemb, (char*)ptr);
out:
    return res;
}

int fseek(int fd, int offset, FILE_SEEK_MODE whence){
    int res = 0;
    struct file_descriptor* desc = file_get_descriptor(fd);
    if(!desc){
        res = -EIO;
        goto out;
    }
    if(whence < SEEK_SET || whence > SEEK_END){
        res = -EINVARG;
        goto out;
    }
    res = desc->filesystem->seek(desc->private, offset, whence);
out: 
    return res;
}


