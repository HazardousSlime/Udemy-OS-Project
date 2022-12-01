#include "heap.h"
#include "memory/memory.h"
#include "status.h"
#include <stdbool.h>

static bool heap_validate_alignment(void *ptr){
    return ((unsigned int)ptr % PEACHOS_HEAP_BLOCK_SIZE) == 0;
}

static int heap_table_validate(void* start_addr, void* end_addr, struct heap_table* table){
    size_t heap_size = (size_t)(end_addr - start_addr);
    size_t table_size = heap_size / PEACHOS_HEAP_BLOCK_SIZE;
    if(table->total != table_size){
        return -EINVARG;
    }
    return PEACHOS_ALL_OK;
}

int heap_create(struct heap* heap, void* start_addr, void* end_addr, struct heap_table* table){
    if(!heap_validate_alignment(start_addr) || !heap_validate_alignment(end_addr)){
        return -EINVARG;
    }

    bzero(heap, sizeof(struct heap));
    heap->start_addr = start_addr;
    heap->table = table;

    if(heap_table_validate(start_addr, end_addr, table) < 0){
        return -EINVARG;
    }
    bzero((void*)(table->entries), sizeof(heap_block_table_entry) * table->total);

    return PEACHOS_ALL_OK;
}

void heap_free(void *ptr, struct heap* heap/*, struct heap_table* table*/){
    //Raise exception if memory address out of range?
    //Raise exception on double free?
    //... belongs to another process?
    //... address not aligned?
    struct heap_table* table = heap->table;
    int start_block = ((uint32_t)ptr - (uint32_t)(heap->start_addr)) / PEACHOS_HEAP_BLOCK_SIZE; 
    int i = start_block;
    for(;table->entries[i] & HEAP_BLOCK_HAS_NEXT; ++i)
        table->entries[i] = 0;
    table->entries[i] = 0;
}



void* heap_malloc(size_t sz, struct heap* heap/*, struct heap_table* table*/){
    struct heap_table* table = heap->table;
    int blocks_required = sz / PEACHOS_HEAP_BLOCK_SIZE + 1;
    int blocks_found = 0;
    int start_block = 0;
    //Find consecutive blocks.
    for(int i = 0; i < table->total && blocks_found < blocks_required; ++i){
        if((table->entries[i] & 0x0F) == HEAP_BLOCK_TABLE_ENTRY_FREE){
            if(blocks_found == 0){
                start_block = i;
            }
            ++blocks_found;
        }else{
            blocks_found = 0;
        }
    }

    if(blocks_found < blocks_required)
        return NULL;
    //Mark first block
    table->entries[start_block] = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    int n = start_block + blocks_required - 1;
    //Mark next blocks
    for(int j=start_block; j < n; ++j)
        table->entries[j] |= HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_HAS_NEXT;
    table->entries[n] = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
    return heap->start_addr + start_block * PEACHOS_HEAP_BLOCK_SIZE;
}