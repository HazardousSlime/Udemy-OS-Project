#ifndef DISK_H
#define DISK_H

typedef unsigned int peachos_disk_type;
#define PEACHOS_DISK_TYPE_REAL  0                               //A real, physical disk
struct disk{
    peachos_disk_type type;
    int sector_size;
};

struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void*buffer);
//int disk_read_sector(int lba, int total, void* buffer);
void disk_search_and_init();
#endif //DISK_H
