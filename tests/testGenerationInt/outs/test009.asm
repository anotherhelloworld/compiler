bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d%c%d', 0
section .text
_main:
    sub esp, 12
    push 10
    push 20
    pop ebx
    pop eax
    imul eax, ebx
    push eax
    push 99
    pop ebx
    pop eax
    xor edx, edx
    div ebx
    push edx
    push ' '
    push 10
    push 20
    pop ebx
    pop eax
    add eax, ebx
    push eax
    push 30
    push 30
    pop ebx
    pop eax
    xor edx, edx
    div ebx
    push eax
    pop ebx
    pop eax
    add eax, ebx
    push eax
    push format0
    call _printf
    add esp, 28
    mov eax, 0
    ret
