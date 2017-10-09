bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
section .text
c1a140363722459944:
    push ebp
    mov ebp, esp
    sub esp, 8
    push 1
    mov eax, ebp
    add eax, 8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    add esp, 8
    pop ebp
    ret 0

_main:
    mov eax, 0
    ret
