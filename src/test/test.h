#ifndef TEST_H
#define TEST_H

#define assert(c) assert_func((c), (__FILE__), (__LINE__), (__func__) )
void assert_func(bool cond, const char *file, int line, const char* func);

#endif //TEST_H