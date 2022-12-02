#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "status.h"
#include "memory/memory.h"

#define BLUE 1
#define BLACK 0
#define WHITE 15

extern void problem();

static uint16_t* video_mem = 0;
static int term_pos = 0;

static struct paging_4gb_chunk* kernel_chunk;

uint16_t terminal_make_char(char c, char colour){
	return (colour << 8) | c;
}

void terminal_put_char(char c, char colour, int x, int y){
	video_mem[y * VGA_WIDTH + x] = terminal_make_char(c, colour);
}

void terminal_put_string(const char* str, char colour, int x, int y){
	int i = 0, start = y * VGA_WIDTH + x;
	while(str[i] != 0){
		video_mem[start++] = terminal_make_char(str[i++], colour);
	}
}

void print(const char* str){
	int i = 0;
	while(str[i] != 0){
		if(str[i] == '\n'){
			term_pos += VGA_WIDTH - (term_pos % VGA_WIDTH);
			++i;
		}
		else
			video_mem[term_pos++] = terminal_make_char(str[i++], WHITE);
	}
}

void terminal_initialize(){
	video_mem = (uint16_t*)(0xB8000);
	for(int y = 0; y < VGA_HEIGHT; ++y){
		for(int x = 0; x < VGA_WIDTH; ++x){
			terminal_put_char(' ',0,x,y);
		}
	}
}

void kernel_main(){
	terminal_initialize();
	disk_search_and_init();
	idt_init();

	kheap_init();

	kernel_chunk = NULL;
	uint8_t flags = PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL;
	kernel_chunk = paging_new_4gb(flags);
	paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

	enable_paging();


	/*struct path_root* test = NULL;
	int res = path_parse("0:/test/file.txt", test);
	switch(res){
		case -EINVARG:
			print("invalid argument");
			break;
		case -EMEMORY:
			print("out of memory");
			break;
		default:
			print("Successful parse");
			break;
	}*/

	char* str = itoa(-2147483648);
	print(str);
	char *str2 = itoa(2147483647);
	print (str2);
	char *str3 = itoa(0);
	print(str3);

	enable_interrupts();
	print("OK");
}
