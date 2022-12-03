#include "streamer.h"
#include "memory/heap/kheap.h"
#include <stddef.h>
#include "status.h"
#include "config.h"

void diskstreamer_close(struct disk_stream* stream){
    kfree(stream);
}

struct disk_stream* diskstreamer_new(int disk_id){
    struct disk* disk = disk_get(disk_id);
    if(!disk)
        return NULL;
    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->disk = disk;
    streamer->pos = 0;
    return streamer;
}

int diskstreamer_seek(struct disk_stream* stream, int pos){
    stream->pos = pos;
    return PEACHOS_ALL_OK;
}

int diskstreamer_read(struct disk_stream* stream, void* out, int total){
    if(total == 0)
        return PEACHOS_ALL_OK;
    if(total < 0)
        return -EINVARG;
    int t = total + PEACHOS_SECTOR_SIZE;
    int res = PEACHOS_ALL_OK;
    do{
        t = t - PEACHOS_SECTOR_SIZE;

        int sector = stream->pos / PEACHOS_SECTOR_SIZE;
        int offset = stream->pos % PEACHOS_SECTOR_SIZE;
        //int sectors = total / PEACHOS_HEAP_BLOCK_SIZE;
        char buf[PEACHOS_SECTOR_SIZE];
        res = disk_read_block(stream->disk, sector, 1, buf);
        if(res)
            goto out;
        int total_to_read = t > PEACHOS_SECTOR_SIZE ? PEACHOS_SECTOR_SIZE : t;
        for(int i = 0; i < total_to_read; ++i){
            *(char*)out++ = buf[offset+i];
        }
        stream->pos += total_to_read;
    }while(t > PEACHOS_SECTOR_SIZE);
out:
    return res;
}