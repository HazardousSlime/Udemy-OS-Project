#include "test/test.h"
#include "fs/pparser.h"
#include "memory/memory.h"
#include "status.h"
#include "kernel.h"

static struct path_root roots[10];

void test_path_parser(){

	//bzero(roots, sizeof(struct path_root*) * 10);

	assert( (path_parse("1:/", "", &roots[0]) == PEACHOS_ALL_OK)); 
	assert( (path_parse("17:/", "", &roots[1]) == -EBADPATH)); 
	assert( (path_parse("7:/test/path.txt/", "", &roots[2]) == PEACHOS_ALL_OK)); 
	assert( (path_parse("5:/this/is/another/path", "", &roots[3]) == PEACHOS_ALL_OK)); 
	assert( (path_parse("5:/this//is/another/path", "", &roots[4]) == -EBADPATH)); 
    TEST_OK;
}

void test_path_traverse(){
    char output[50];
    //char buf[5];
    path_traverse(&roots[0], output);
    assert(( memcmp(output, "1:/", 3) == 0));
    //puts(output);
    path_traverse(&roots[2], output);
    assert(( memcmp(output, "7:/test/path.txt/", strlen("7:/test/path.txt/")) == 0));
    //puts(itoa(memcmp(output, "7:/test/path.txt/", strlen("7:/test/path.txt/")), buf));      
    //puts(output);
    path_traverse(&roots[3], output);
    assert(( memcmp(output, "5:/this/is/another/path/", strlen("5:/this/is/another/path/")) == 0));
    //puts(output);
    TEST_OK;
}