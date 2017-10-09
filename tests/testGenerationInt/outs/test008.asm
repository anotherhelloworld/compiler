bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 0
section .text
_main:
    sub esp, 4
    push 10
    push 2
    pop ebx
    pop eax
    imul eax, ebx
    push eax
    push 3
    pop ebx
    pop eax
    xor edx, edx
    div ebx
    push edx
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
