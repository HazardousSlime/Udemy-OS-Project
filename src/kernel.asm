[BITS 32]

global _start
global problem
;global problem_escape
extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10
_start:
	mov ax, DATA_SEG			;Configure segments (Selective Memory Model)
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

								;Enable the A20 line
	in al, 0x92					;Read from port 0x92 on the processor bus
	or al, 2					;Enable bit 1
	out 0x92, al				;Write value back

	mov al, 00010001b			;Begin Remap the master programmable interrupt controller
	out 0x20, al				;Send to port 0x20

	mov al, 0x20
	out 0x21, al				;Master PIC interrupts should start ISR 0x20

	mov al, 00000001b			;End remap
	out 0x21, al

	;sti							;Make sure interrupts are enabled

	call kernel_main
	jmp $

problem:
	mov eax, 0
	div eax
	;int 0
;problem_escape:
	ret

times 512 - ($- $$) db 0			;Pad Sector with zeroes
						;This prevents alignment conflicts with C code
						;This section must appear at the beginning of the
						;kernel code so it can't be put in the .asm section
;===========================================================================
; *Notes
;==========================================================================
;add-symbol-file build/kernelfull.o 0x0100000
