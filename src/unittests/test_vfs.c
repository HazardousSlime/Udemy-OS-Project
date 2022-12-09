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
    assert(fseek(fd, 2, SEEK_SET) == PEACHOS_ALL_OK);
    assert(fread(buf, 1, 3, fd) == 3);
    assert(memcmp("llo", buf, 3) == 0);
    TEST_OK;
}

void test_fstat(){
    struct file_stat stat;
    assert(fstat(fd, &stat) == PEACHOS_ALL_OK);
    assert(stat.filesize == 6);
    assert(stat.flags == 0);
    assert(fstat(7, &stat) == -EIO);
    TEST_OK;
}

void test_fclose(){
    assert(fclose(fd) == PEACHOS_ALL_OK);
    assert(fclose(fd) == -EIO);
    assert(fclose(7) == -EIO);
    //Make sure the file descriptor becomes usable again
    assert(fopen("0:/h", "r") == 1);
    assert(fopen("0:/h", "r") == 2);
    assert(fopen("0:/h", "r") == 3);
    assert(fclose(2) == PEACHOS_ALL_OK);
    assert(fopen("0:/h", "r") == 2);
    assert(fclose(1) == PEACHOS_ALL_OK);
    assert(fopen("0:/h", "r") == 1);
    TEST_OK;
}