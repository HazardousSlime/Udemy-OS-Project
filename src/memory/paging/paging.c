#include "paging.h"
#include "memory/heap/kheap.h"
#include <stddef.h>
//#include <stdbool.h>
#include "status.h"

directory_entry* current_directory = NULL;

extern void paging_load_directory(directory_entry* directory);

directory_entry* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk){
    return chunk->directory;
}

//Calculate index into directory and index into page table for that entry.
int paging_get_indices  (   void* virtual_address, 
                            uint32_t* dir_index_out, 
                            uint32_t* page_index_out
                        ){
    int res = PEACHOS_ALL_OK;
    if(!paging_is_aligned(virtual_address)){
        res = -EINVARG;
        goto out;
    }
    uint32_t _4M = PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
    *dir_index_out = ((uint32_t)(virtual_address) / _4M);
    *page_index_out = ((uint32_t)(virtual_address) % _4M / PAGING_PAGE_SIZE);
out:
    return res;
}

bool paging_is_aligned(void* addr){
    return ((uint32_t)addr % PAGING_PAGE_SIZE) == 0;
}

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags){
    size_t directory_size = sizeof(directory_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE;
    directory_entry* directory = (directory_entry*)kzalloc(directory_size);
    int offset = 0;
    int _4M = PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; ++i, offset += _4M){
        size_t table_size = sizeof(page_table_entry) * PAGING_TOTAL_ENTRIES_PER_TABLE;
        page_table_entry* page_table = kzalloc(table_size);
        for(int b = 0; b < PAGING_TOTAL_ENTRIES_PER_TABLE; ++b){
            page_table[b] = (offset + (b * PAGING_PAGE_SIZE)) | flags;
        }
        //Write access is determined per page table
        directory[i] = (directory_entry)page_table | flags | PAGING_IS_WRITABLE;
    }
    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory = directory;
    return chunk_4gb;
}

//Set page table entry (physical address + flags) for virtual address
int paging_set(directory_entry* directory, void* virtual_address, page_table_entry val){
    int res = PEACHOS_ALL_OK;
    if(!paging_is_aligned(virtual_address)){
        res = -EINVARG;
        goto out;
    }
    uint32_t directory_index = 0;
    uint32_t  table_index = 0;
    res = paging_get_indices(virtual_address, &directory_index, &table_index);
    if(res < 0)
        goto out;
    directory_entry dir_entry = directory[directory_index];
    page_table_entry* table = (page_table_entry*)(dir_entry & 0xfffff000);
    table[table_index] = val;
out:
    return res;
}

void paging_switch(directory_entry* directory){
    paging_load_directory(directory);
    current_directory = directory;
}