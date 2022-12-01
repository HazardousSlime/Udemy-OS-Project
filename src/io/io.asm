section .asm

global insb
global insw
global outb
global outw

insb:
    push ebp
    mov ebp, esp

    xor eax, eax            ;Zero the a register
    mov edx, [ebp+8]        ;Put the port number (short) into dx
    in al, dx               ;Read byte into al
                            ;Return value is stored in A register by default
    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax            ;Zero the a register
    mov edx, [ebp+8]        ;Put the port number (short) into dx
    in ax, dx               ;Read word into ax
                            ;Return value is stored in A register by default
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    ;xor eax, eax            ;Zero the a register
    mov edx, [ebp+8]        ;Put the port number (short) into dx
    mov eax, [ebp+12]        ;Put byte to send into al
    out dx, al              ;send byte stored in al
                            ;Return value is stored in A register by default
    pop ebp

    ret

outw:
    push ebp
    mov ebp, esp

    ;xor eax, eax            ;Zero the a register
    mov edx, [ebp+8]        ;Put the port number (short) into dx
    mov eax, [ebp+12]        ;Put byte to send into al
    out dx, ax              ;send word stored in ax
                            ;Return value is stored in A register by default
    pop ebp
    ret

    ;=========================================================================================
    ; Notes
    ;=========================================================================================
    ;Parameters always take up four bytes on the stack
    ;The function call instruction always pushes the return address
    ;Pushing the base pointer means the saved ebp will be pointer to by [ebp+0]
    ;The return address is at [ebp+4]
    ;Variables start at [ebp+8], [ebp+12], [ebp+16], etc.