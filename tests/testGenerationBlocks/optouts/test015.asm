;6
;10
bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
    vmax: dd 20
    va: times 20 dd 20, 10, 11, 12, 13, 19, 18, 17, 16, 15, 14, 1, 2, 3, 6, 4, 5, 7, 9, 8
    vi: dd 0
section .text
c3quicksort140665746883176:
    push ebp
    mov ebp, esp
    sub esp, 20
    add esp, 20
    pop ebp
    ret 12

_main:
    mov eax, depth
    mov [eax], ebp
    mov eax, va
    push eax
    push 1
    mov eax, vmax
    push dword [eax]
    call c3quicksort140665746883176
    push 1
    mov eax, vi
    pop ebx
    mov [eax], ebx
    .L1: 
    mov eax, vi
    push dword [eax]
    mov eax, vmax
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jg .L3
    push 0
    jmp .L4
    .L3: 
    push 1
    .L4: 
    pop eax
    test eax, eax
    jnz .L2
    sub esp, 4
    mov eax, va
    push eax
    mov eax, vi
    push dword [eax]
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    mov eax, vi
    push dword [eax]
    mov ebx, 1
    add [esp], ebx
    mov eax, vi
    pop ebx
    mov [eax], ebx
    jmp .L1
    .L2: 
    mov eax, 0
    ret
