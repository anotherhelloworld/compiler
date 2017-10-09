bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
section .text
c1a140367640181432:
    push ebp
    mov ebp, esp
    sub esp, 8
    push 10
    mov eax, ebp
    add eax, 8
    pop ebx
    mov [eax], ebx
    add esp, 8
    pop ebp
    ret 0

_main:
    sub esp, 8
    mov eax, depth
    mov [eax], ebp
    call c1a140367640181432
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
