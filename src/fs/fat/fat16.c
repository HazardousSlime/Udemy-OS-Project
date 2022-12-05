#include "fat16.h"
#include "status.h"
#include "memory/memory.h"
#include <stddef.h>
#include <stdint.h>

#define PEACHOS_FAT16_SIGNATURE 0x29
#define PEACHOS_FAT16_ENTRY_SIZE 0x02
#define PEACHOS_FAT16_BAD_SECTOR 0xFF7
#define PEACHOS_FAT16_UNUSED 0x00

typedef unsigned int FAT_ITEM_TYPE;
#define FAT_ITEM_TYPE_DIRECTORY 0
#define FAT_ITEM_TYPE_FILE 1

//Directory entry attributes bitmask
#define FAT_FILE_READ_ONLY 0x01
#define FAT_FILE_HIDDEN 0x02
#define FAT_FILE_SYSTEM 0x04
#define FAT_FILE_VOLUME_LABEL 0x08
#define FAT_FILE_SUBDIRECTORY 0x10
#define FAT_FILE_ARCHIVED 0x20
#define FAT_FILE_DEVICE 0x40
#define FAT_FILE_RESERVED 0x80

struct fat_header_extender{
    uint8_t driver_number;
    uint8_t win_nt_bit;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_id_string[11];
    uint8_t system_id_string[8];
}__attribute__((packed));           //packed structure ensures that struct fields do not get rearranged or padded by gcc

struct fat_header{
    uint8_t short_jmp_ins[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t sectors_big;
}__attribute__((packed));

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