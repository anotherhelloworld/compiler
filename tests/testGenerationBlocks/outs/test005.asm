bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%s', 10, 0
    str0: db 'Hello world!', 0
section .text
_main:
    .L1: 
    push 1
    pop eax
    test eax, eax
    jz .L2
    sub esp, 4
    push str0
    push format0
    call _printf
    add esp, 12
    jmp .L2
    jmp .L1
    .L2: 
    mov eax, 0
    ret
