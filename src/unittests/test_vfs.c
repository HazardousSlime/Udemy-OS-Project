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