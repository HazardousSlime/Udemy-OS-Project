#include "kheap.h"
#include "heap.h"
#include "kernel.h"
#include "memory/memory.h"
//osdev.org memory map

struct heap kernel_heap;
struct heap_table kernel_heap_table;
void kheap_init()
{
    int total_table_entries = PEACHOS_HEAP_SIZE_BYTES / PEACHOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (heap_block_table_entry*)(PEACHOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;
    void* end = (void*)(PEACHOS_HEAP_ADDRESS + PEACHOS_HEAP_SIZE_BYTES);
    int res = heap_create(&kernel_heap, PEACHOS_HEAP_ADDRESS, end, &kernel_heap_table);
    if (res < 0){
        print("Failed to create heap\n");
        //Panic
    }
}

void kfree(void* ptr){
    heap_free(ptr, &kernel_heap);
}

void* kmalloc(size_t sz){
    return heap_malloc(sz, &kernel_heap);
}

void* kzalloc(size_t sz){
    void* ptr = kmalloc(sz);
    if(ptr == NULL){
        return NULL;
    }
    bzero(ptr, sz);
    return ptr;
}