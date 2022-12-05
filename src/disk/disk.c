#include "io/io.h"
#include "status.h"
#include "disk/disk.h"
#include "memory/memory.h"
#include "config.h"
#include <stddef.h>

static struct disk dsk;

int disk_read_sector(int lba, int total, void* buffer);

int disk_read_block(struct disk* idisk, unsigned int lba, int total, void*buffer){
    if(idisk != &dsk)
        return -EIO;
    return disk_read_sector(lba, total, buffer);
}

int disk_read_sector(int lba, int total, void* buffer){
    outb(0x1f6, (lba >> 24) | 0xe0);                        //send upper byte of lba w/ upper 3 bits on
    outb(0x1f2, total);                                     //send total sectors to read
    outb(0x1f3, (unsigned char)(lba & 0xff));               //send lowest byte of lba
    outb(0x1f4, (unsigned char)(lba >> 8));                 //send bits 8-15    
    outb(0x1f5, (unsigned char)(lba >> 16));                //send bits 16-23
    outb(0x1f7, 0x20);                                      //initiate read

    unsigned short* ptr = (unsigned short*) buffer;         //read two bytes at a time
    for(int b = 0; b < total; ++b){
        while((insb(0x1f7) & 0x08) == 0);                   //wait until sector is ready to be read
        for(int i = 0; i < 256; ++i)                        //read sector
            ptr[i] = insw(0x1f0);        
    }
    return PEACHOS_ALL_OK;
}

struct disk* disk_get(int index){
    if(index != 0)
        return NULL;
    return &dsk;
}

void disk_search_and_init(){
    bzero(&dsk, sizeof(struct disk));
    dsk.sector_size = PEACHOS_SECTOR_SIZE;
    dsk.type = PEACHOS_DISK_TYPE_REAL;
    dsk.filesystem = fs_resolve(&dsk);
}

