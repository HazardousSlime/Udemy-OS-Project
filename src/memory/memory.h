#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdbool.h>

void bzero(void *ptr, size_t sz);
int char_to_digit(char c);
int memcmp(const void* str1, const void* str2, size_t n);
void memset(void* ptr, int c, size_t sz);
void strncpy(void* dest, const void* src, size_t sz);
bool is_digit(char c);
int istrncmp(const char* str, const char* str2, int n);
char* itoa(int num, char* buf);
int strlen(const char *str);
int strnlen_terminator(const char* str, int max, char terminator);

#endif //MEMORY_H