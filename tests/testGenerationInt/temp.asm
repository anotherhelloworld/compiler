bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 0
section .text
_main:
    sub esp, 4
    mov eax, 3
    mov ebx, 2
    and eax, ebx
    push eax
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
