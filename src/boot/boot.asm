;===========================================================================
;* Boot Sector Programs							             
;===========================================================================

ORG 0x7c00					;Default value of CS is 0x7c0
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
KERNEL_START equ 0x0100000

_start:						;Dummy BIOS Parameter block
	jmp short start
	nop

times 33 db 0

start:
	jmp 0:step2				;offset relative to origin of code is 0

step2:						;Disable interrupts
	cli
	mov ax, 0x0
	mov ds, ax				;Set data segment
	mov es, ax				;Set extra segment
	mov ss, ax				;Set Stack bottom
	mov sp, 0x7c00				;Set stack pointer
	sti					;Re-enable interrupts

.load_protected:
	cli					;Disable interrupts
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:load32			;Note: The "Code segment" is not that same as CS. It
	;jmp $					;is a selector which is an offset into the GDT (0x08), not a 
						;memory address or segment pointer.


							;Setting the segment limit to 0xFFFF and granularity to page
							;Makes all 4 GB of RAM available for use
							;Base contains the base address of the code/data segments
							;Refer to osdev.org for details regarding flags/access byte

gdt_start:
gdt_null:					;Global Descriptor Table
	dd 0x0					;64 bits null
	dd 0x0
gdt_code:					;Offset 0x8 (CS points here)
	dw 0xffff				;Segment limit first 0-15 bits				
	dw 0					;Base first 0-15 bits
	db 0					;Base first 16-23 bits
	db 0x9a					;Access Byte	(10011010b)
	db 11001111b				;High 4 bit flags and low 4 bits flags
	db 0					;Base 24-31 bits

gdt_data:					;Offset 0x10 
	dw 0xffff				;Segment limit first 0-15 bits				
	dw 0					;Base first 0-15 bits
	db 0					;Base first 16-23 bits
	db 0x92					;Access Byte (10010010b)
	db 11001111b				;High 4 bit flags and low 4 bits flags
	db 0					;Base 24-31 bits

gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start
	dd gdt_start

[BITS 32]
load32:
	mov eax, 1				;Load starting at sector 1
	mov ecx, 100				;Load 100 sectors
	mov edi, KERNEL_START			;Place data at 0x0100000
	call ata_lba_read
	jmp KERNEL_START

ata_lba_read:
	mov ebx, eax				;Store LBA (sector 1) temporarily
	shr eax, 24				;Send the highest 8 bits of the lba to disk controller
	or eax, 0xe0				;Selects master drive
	mov dx, 0x1f6				;Place port number in dx
	out dx, al				;Send bits to disk controller

	mov eax, ecx				
	mov dx, 0x1f2
	out dx, al				;Send number of sectors to controller

	mov eax, ebx
	mov dx, 0x1f3
	out dx, al				;Send bits 0-7

	mov eax, ebx
	shr eax, 8
	mov dx, 0x1f4
	out dx, al				;Send bits 8-15

	mov eax, ebx
	shr eax, 16
	mov dx, 0x1f5
	out dx, al				;Send bits 16-23

	mov dx, 0x1f7
	mov al, 0x20
	out dx, al

.next_sector:
	push ecx

.try_again:
	mov dx, 0x1f7
	in al, dx
	test al, 8				;Test to see if a bit is set
	jz .try_again

	mov ecx, 256				;One sector = 256 words (16-bit numbers)
	mov dx, 0x1f0				
	rep insw				;Read 256 16-bit values from the disk and store in [ES:EDI]
	pop ecx
	loop .next_sector			;ecx is the 'count' register which decrements automatically

	ret
	
times 510 - ($- $$) db 0			;Pad Sector with zeroes
dw 0xAA55					;Final word must be 0xAA55

;===========================================================================
; *Notes
;==========================================================================
;	[Links]
;osdev.org
;Ralf Brown's Interrupt List
;
;	[Build Instructions]
;nasm -f bin <input> -o <output>

;	[Run]
;qemu-system-x86_64 <output>

;	[install]
;sudo fdisk -l (list partitions)
;sudo dd if=<input> of=<partition> 

;	[Interrupts]
;Interrupts have a number associated with them
;Interrupt routine addresses are stored in a vector table
;Table entries are four bytes [offset:segment]
;The Table is starting at 0x00 (entry n stored at 4*n)
;Exceptions can call interrupts (such as division by zero)

;	[Filesystems]
;File systems are not implemented in the BIOS.
;A kernel requires a filesystem driver to understand disk formats
;The BIOS can read and write to disk sectors which are typically 512 bytes
;CHS (Cylinder Head Sector) has been replaced by LBA (Logical Block Address)
;LBA treats a disk like an array of sectors
;[Reading byte 58376 on the Disk]
;LBA = 58376/512 = 114
;Load sector into a buffer
;Offset = 58376 % 512 = 8
;In real mode, interrupt 13h is for disk operations
;
;	[Protected Mode]
;Ring 0 - Kernel mode
;Ring 3 - user mode, no direct hardware access or sti/cli
;Code segments become selectors
;Paging (remapping memory addresses)
;[Selective Memory]
;Segment registers point to data structures containing memory ranges and permission levels
;[Paging Memory]
;With paging, only the assigned memory address range is visible to a user program
;	[Debugging]
;info registers
;layout asm
