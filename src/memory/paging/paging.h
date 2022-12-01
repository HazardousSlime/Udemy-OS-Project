#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stdbool.h>

#define PAGING_CACHE_ENABLED            0b00010000
#define PAGING_WRITE_THROUGH            0b00001000
#define PAGING_ACCESS_FROM_ALL          0b00000100      //Accessible from all privilege levels
#define PAGING_IS_WRITABLE              0b00000010
#define PAGING_IS_PRESENT               0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_PAGE_SIZE 4096

typedef uint32_t directory_entry;
typedef uint32_t page_table_entry;

struct paging_4gb_chunk{
    directory_entry* directory;
};

void enable_paging();
directory_entry* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk);
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags);
bool paging_is_aligned(void* addr);
int paging_set(directory_entry* directory, void* virtual_address, page_table_entry val);
void paging_switch(directory_entry* directory);

#endif //PAGING_H