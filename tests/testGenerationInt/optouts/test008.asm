;5
;6
;7
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 0
section .text
_main:
    sub esp, 4
    mov eax, 10
    imul eax, 2
    push eax
    mov ebx, 3
    pop eax
    xor edx, edx
    div ebx
    push edx
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
