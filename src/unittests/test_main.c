#include "unittests/test_parser.h"
#include "unittests/test_streamer.h"
#include "unittests/test_vfs.h"

void test_main_run_tests(){
    //Parser
    test_path_parser();
    test_path_traverse();
    //Streamer
    test_streamer();
    test_fopen();
    test_fread();
    test_fseek();
    test_fstat();
    test_fclose();
}