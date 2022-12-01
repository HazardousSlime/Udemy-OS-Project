#include "memory.h"

/*const int BLOCK_SIZE = 4096;
const int NUMBER_OF_BLOCKS = 1024 * 1024 / BLOCK_SIZE;
const int HEAP_START_ADDRESS = 0x01000000;
const int BLOCK_UNUSED = 0;
const int BLOCK_USED = 0x0F;
const int HAS_N = 0x80;
const int IS_FIRST = 0x40;

//Allocation table entry
//Bit 7: HAS_N - set if next block is part of allocation
//Bit 6: IS_FIRST - set if first entry in allocation
//Bits 4+5: Unused
//Bits 0-3: Entry type (taken or free)

unsigned char* heap_entry_table = NULL;*/

void bzero(void *ptr, size_t sz){
    memset(ptr,0,sz);
}

/*void free(void *ptr){
    //Raise exception if memory address out of range?
    //Raise exception on double free?
    //... belongs to another process?
    int start_block = ((uint32_t)ptr - HEAP_START_ADDRESS) / BLOCK_SIZE; 
    for(int i = start_block; heap_entry_table[i] & HAS_N; ++i)
        heap_entry_table[i] = 0;
}

int heap_init(){
    bzero(heap_entry_table, NUMBER_OF_BLOCKS);
}

void* malloc(size_t sz){
    int blocks_required = sz / BLOCK_SIZE + 1;
    int blocks_found = 0;
    int start_block = 0;
    //Find consecutive blocks.
    for(int i = 0; i < NUMBER_OF_BLOCKS && blocks_found < blocks_required; ++i){
        if(heap_entry_table[i] & 0x0F == BLOCK_UNUSED){
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
    heap_entry_table[start_block] |= IS_FIRST;
    int n = start_block + blocks_required - 1;
    //Mark next blocks
    for(int j=start_block; j < n; ++j)
        heap_entry_table[j] |= HAS_N;
    return HEAP_START_ADDRESS + start_block * BLOCK_SIZE;
}*/

void memset(void* ptr, int c, size_t sz){
    char* c_ptr = (char *)ptr;
    for(int i = 0; i < sz; ++i){
        c_ptr[i] = (char)c;
    }
}

void strncpy(void* dest, const void* src, size_t sz){
    char* c_dest = (char *)dest;
    char* c_src = (char *)src;
    int n = sz + 1;
    for(int i = 0; i < n; ++i){
        c_dest[i] = c_src[i];
    }
}
