#include "test/test.h"
#include "status.h"
#include "fs/file.h"

void test_fopen(){
    assert(fopen("0:/h", "r"));
    TEST_OK;
}