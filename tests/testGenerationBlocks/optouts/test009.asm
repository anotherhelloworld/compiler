;6
;10
;6
;10
;6
;6
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d%c%d', 10, 0
    vi: dd 0
    vj: dd 0
section .text
_main:
    push 1
    mov eax, vi
    pop ebx
    mov [eax], ebx
    .L1: 
    mov eax, vi
    push dword [eax]
    mov ebx, 4
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
    push 1
    mov eax, vj
    pop ebx
    mov [eax], ebx
    .L5: 
    mov eax, vj
    push dword [eax]
    mov ebx, 4
    pop eax
    cmp eax, ebx
    jg .L7
    push 0
    jmp .L8
    .L7: 
    push 1
    .L8: 
    pop eax
    test eax, eax
    jnz .L6
    sub esp, 12
    mov eax, vj
    push dword [eax]
    push ' '
    mov eax, vi
    push dword [eax]
    push format0
    call _printf
    add esp, 28
    mov eax, vj
    push dword [eax]
    mov ebx, 1
    add [esp], ebx
    mov eax, vj
    pop ebx
    mov [eax], ebx
    jmp .L5
    .L6: 
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
