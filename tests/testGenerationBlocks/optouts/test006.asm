;6
;10
;6
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    vi: dd 5
section .text
_main:
    .L1: 
    mov eax, vi
    push dword [eax]
    mov ebx, 1
    add [esp], ebx
    mov eax, vi
    pop ebx
    mov [eax], ebx
    sub esp, 4
    mov eax, vi
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    jmp .L3
    jmp .L2
    .L3: 
    mov eax, vi
    push dword [eax]
    mov ebx, 10
    pop eax
    cmp eax, ebx
    je .L4
    push 0
    jmp .L5
    .L4: 
    push 1
    .L5: 
    pop eax
    test eax, eax
    jz .L1
    .L2: 
    mov eax, 0
    ret
