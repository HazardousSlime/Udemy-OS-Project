#include "test/test.h"
#include "status.h"
#include "fs/file.h"
#include "memory/memory.h"
#include "kernel.h"

static int fd = -1;

void test_fopen(){
    assert(fd = fopen("0:/h", "r"));
    TEST_OK;
}

void test_fread(){
    char buf[6];
    bzero(buf, sizeof(buf));
    //Read one array of length five
    assert(fread(buf, sizeof(buf)-1, 1, fd) == 1);
    assert(memcmp("hello", buf, sizeof(buf) - 1) == 0);
    TEST_OK;
}

void test_fseek(){
    char buf[6];
    bzero(buf, sizeof(buf));
    assert(fseek(fd, -1, SEEK_SET) == -EIO);        
    assert(fseek(fd, 12, SEEK_SET) == -EIO);
    assert(fseek(fd, 0, SEEK_SET) == PEACHOS_ALL_OK);
    assert(fseek(fd, 0, SEEK_END) == -EUNIMP);
    assert(fseek(7, 0, SEEK_SET) == -EIO);
    assert(fseek(fd, 0, 473) == -EINVARG);
    assert(fseek(fd, 1, SEEK_SET) == PEACHOS_ALL_OK);
    assert(fseek(fd, 3, SEEK_CUR) == PEACHOS_ALL_OK);
    assert(fseek(fd, 2, SEEK_CUR) == -EIO);

    TEST_OK;
}