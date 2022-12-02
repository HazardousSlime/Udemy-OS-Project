#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

#define assert(c) assert_func((c), (__FILE__), (__LINE__), (__func__) )
void assert_func(bool cond, const char *file, int line, const char* func);
#define TEST_OK test_ok_func((__func__))
void test_ok_func(const char* func);
#endif //TEST_H