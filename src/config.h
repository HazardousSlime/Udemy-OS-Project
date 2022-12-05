#ifndef CONFIG_H
#define CONFIG_H

#define PEACHOS_TOTAL_INTERRUPTS 512

//Remember to redefine these constants in kernel.asm and boot.asm
#define KERNEL_CODE_SELECTOR 0x08
#define KERBEL_DATA_SELECTOR 0x10

#define PEACHOS_HEAP_SIZE_BYTES 1024*1024*100
#define PEACHOS_HEAP_BLOCK_SIZE 4096
#define PEACHOS_HEAP_ADDRESS 0x01000000
#define PEACHOS_HEAP_TABLE_ADDRESS 0x00007e00

#define PEACHOS_SECTOR_SIZE 512

#define PEACHOS_MAX_FILESYSTEMS 12
#define PEACHOS_MAX_FILE_DESCRIPTORS 512

#endif //CONFIG_H