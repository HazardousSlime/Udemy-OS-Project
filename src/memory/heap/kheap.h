#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>

void kheap_init();
void kfree(void* ptr);
void* kmalloc(size_t sz);
void* kzalloc(size_t sz);

#endif //KHEAP_H