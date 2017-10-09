;2
;5
;10
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 0
section .text
_main:
    sub esp, 4
    push 1
    mov eax, 2
    mov ebx, 3
    xor edx, edx
    div ebx
    mov ebx, edx
    add [esp], ebx
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
