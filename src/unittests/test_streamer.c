#include "disk/streamer.h"
#include "status.h"
#include "test/test.h"

void test_streamer(){
    int disk_no = 0;
    int pos = 0x201;
    unsigned char c = 0;
    struct disk_stream* stream = diskstreamer_new(disk_no);
    diskstreamer_seek(stream, pos);
    //Attempt to read one byte with no errors
    assert(diskstreamer_read(stream, &c, 1) == PEACHOS_ALL_OK);
    //Verify correctness of byte
    assert(c == 184);
    TEST_OK;
}