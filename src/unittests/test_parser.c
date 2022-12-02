#include "test/test.h"
#include "fs/pparser.h"
#include "memory/memory.h"
#include "status.h"

void test_path_parser(){
    struct path_root* roots[10];
	bzero(roots, sizeof(struct path_root*) * 10);

	assert( (path_parse("1:/", roots[0]) == PEACHOS_ALL_OK)); 
	assert( (path_parse("17:/", roots[1]) == -EBADPATH)); 
	assert( (path_parse("7:/test/path.txt/", roots[2]) == PEACHOS_ALL_OK)); 
	assert( (path_parse("5:/this/is/another/path", roots[3]) == PEACHOS_ALL_OK)); 
	assert( (path_parse("5:/this//is/another/path", roots[4]) == -EBADPATH)); 
    TEST_OK;
}