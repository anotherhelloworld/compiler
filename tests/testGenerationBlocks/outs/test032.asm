bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
section .text
c1a140539673747704:
    push ebp
    mov ebp, esp
    sub esp, 8
    push 10
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
    sub esp, 8
    mov eax, depth
    mov [eax], ebp
    call c1a140539673747704
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
