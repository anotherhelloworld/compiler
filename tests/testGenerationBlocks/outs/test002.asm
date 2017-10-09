bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    format1 : db '%s', 10, 0
    str0: db 'hello world!', 0
    va: dd 0
section .text
_main:
    sub esp, 4
    mov eax, va
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    .L1: 
    push 0
    pop eax
    test eax, eax
    jz .L2
    sub esp, 4
    push str0
    push format1
    call _printf
    add esp, 12
    jmp .L1
    .L2: 
    mov eax, 0
    ret
