#ifndef IDT_H
#define IDT_H

//Homework: Research attribute packed

							//Protected Mode Interrupt Descriptor Table Entry

#include <stdint.h>
#include "config.h"

#define INTERRUPT_32_BIT_RING_3 0xEE
#define TRAP_32_BIT_RING_3 0xEF
//11101110b
//bit 7: Use interrupt
//bits 5+6: Ring 3
//bit 4: trap or interrupt gate
//bits 0-3: 0xE (32-bit interrupt)

struct idt_desc
{
	uint16_t offset_1;				//Offset bits 0-15
	uint16_t selector;				//Code segment selector in GDT or LDT
	uint8_t zero;					//Unused, set to 0	
	uint8_t type_attr;				//Type and attributes
	uint16_t offset_2;				//Offset bits 16-31
}__attribute__((packed));				//Do not align structure in memory	

							//There are multiple interrupt gate types but most of them are not needed

//struct idt_desc idt_desc[COS32_MAX_INTERRUPTS];	//The table itself is an array of entries (int 0 runs the interrupt defined by entry 0)

struct idtr_desc					//IDTR
{
	uint16_t limit;					//Size of descriptor table in bytes - 1
	uint32_t base;					//Points to Table
}__attribute__((packed)); 

void disable_interrupts();
void enable_interrupts();
void idt_init();

#endif //IDT_H
