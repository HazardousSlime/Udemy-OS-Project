#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"

struct disk_stream{
    int pos; 
    struct disk* disk;
};

struct disk_stream* diskstreamer_new(int disk_id);
int diskstreamer_seek(struct disk_stream* stream, int pos);
int diskstreamer_read(struct disk_stream* stream, void* out, int total);

#endif //STREAMER_H