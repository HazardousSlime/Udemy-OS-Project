section .asm

extern int21h_handler
extern no_interrupt_handler

global disable_interrupts
global enable_interrupts
global idt_load
global int21h
global no_interrupt

disable_interrupts:
    cli
    ret

enable_interrupts:
    sti
    ret

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]                ;fetch address of idtr descriptor from stack
    lidt [ebx]                      ;load idt

    pop ebp
    ret

no_interrupt:
    cli
    pushad                          ;save general registers
    call no_interrupt_handler
    popad
    sti
    iret

int21h:
    cli
    pushad                          ;save general registers
    call int21h_handler
    popad
    sti
    iret