bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d%c%d', 10, 0
section .text
c0B140348540149016:
    push ebp
    mov ebp, esp
    sub esp, 8
    mov eax, depth
    mov [eax + 4], ebp
    call c0A140348540149128
    add esp, 8
    pop ebp
    ret 0
c0A140348540149128:
    push ebp
    mov ebp, esp
    sub esp, 8
    sub esp, 12
    push 2
    push ' '
    push 1
    push format0
    call _printf
    add esp, 28
    add esp, 8
    pop ebp
    ret 0


_main:
    mov eax, depth
    mov [eax], ebp
    call c0B140348540149016
    mov eax, 0
    ret
