#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void bzero(void *ptr, size_t sz);
void memset(void* ptr, int c, size_t sz);
void strncpy(void* dest, const void* src, size_t sz);
char* itoa(int num);

#endif //MEMORY_H