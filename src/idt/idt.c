#include "idt.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;
struct interrupt_frame;


extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();
/*__attribute__((interrupt))
void idt_zero(struct interrupt_frame *frame){
    //static char c = 0;
    //print(&c);
    //++c;
    print("Divide by zero.");
    
}*/

void int21h_handler(){
    print("Key pressed.\n");
    //acknowledge interrupt
    outb(0x20, 0x20);
}

void idt_set(int interrupt_no, void *address){
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    //Lower 16 bits
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    //Upper 16 bits
    desc->offset_2 = (uint32_t) address >> 16;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0;
    desc->type_attr = INTERRUPT_32_BIT_RING_3;
}

void idt_set_dummy_interrupts(){
    for(int i = 0; i < PEACHOS_TOTAL_INTERRUPTS; ++i)
        idt_set(i, no_interrupt);
}

void idt_init(){
    //Initialize IDT
    bzero(idt_descriptors, sizeof(idt_descriptors));
    //Set up IDTR descriptor
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t)idt_descriptors;
    //Set interrupt zero to idt_zero
    //idt_set(0, idt_zero, INTERRUPT_32_BIT_RING_3);
    //idt_set(0x20, int20h);
    idt_set_dummy_interrupts();
    idt_set(0x21, int21h);
    //Activate IDT
    idt_load(&idtr_descriptor);
    //enable_interrupts();
}

void no_interrupt_handler(){
    outb(0x20, 0x20);
}