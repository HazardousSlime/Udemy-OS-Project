#include "memory.h"
#include "heap/kheap.h"
#include <stdbool.h>

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

int char_to_digit(char c){
    return (int)(c - 48);
}

bool is_digit(char c){
    return c >= 48 && c <= 57;
}

char* itoa(int num, char* buf){
    size_t max_len = 12;        //The maximum length in digits of a signed integer + null terminator is 12
    int stack[max_len];
    buf[0] = '0';
    int b_index = 0;
    if(num == 0){
        ++b_index;
        goto out;
    }

    int top = max_len - 1;
    for(int n = num; n != 0; n = n / 10, --top){
        stack[top] = (num >= 0) ? n % 10 : -1 * (n % 10);
    }
    ++top;
    if(num < 0)
        buf[b_index++] = '-';
    for(;top < max_len;++b_index,++top){
        buf[b_index] = stack[top] + 48;
    }
out:
    buf[b_index] = '\0';
    return buf;
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

int memcmp(const void* str1, const void* str2, size_t n){
    const char* c_str1 = (char*)str1;
    const char* c_str2 = (char*)str2;
    int i = 0;
    //int u = n - 1;
    for(;i < n && c_str1[i] == c_str2[i];++i);
    return c_str1[i] - c_str2[i];
}

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

int strlen(const char *str){
    int i = 0;
    while(str[i++]);
    return i - 1;
}

int strnlen(const char *str, int max){
    int i = 0;
    while(i < max && str[i++]);
    return i;
}