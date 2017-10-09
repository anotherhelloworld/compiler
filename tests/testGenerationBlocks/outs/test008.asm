bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%s', 10, 0
    format1 : db '%s', 0
    str0: db 'world!', 0
    str1: db 'Hello ', 0
section .text
_main:
    jmp Lb
    La: 
    sub esp, 4
    push str0
    push format0
    call _printf
    add esp, 12
    jmp Lc
    Lb: 
    sub esp, 4
    push str1
    push format1
    call _printf
    add esp, 12
    jmp La
    Lc: 
    mov eax, 0
    ret
