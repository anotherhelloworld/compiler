bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '', 10, 0
    format1 : db '%d', 10, 0
    vmax: dd 20
    va: times 20 dd 20, 10, 11, 12, 13, 19, 18, 17, 16, 15, 14, 1, 2, 3, 6, 4, 5, 7, 9, 8
    vi: dd 0
section .text
c2sort140268183823368:
    push ebp
    mov ebp, esp
    sub esp, 16
    push 1
    mov eax, ebp
    add eax, -4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    push 2
    mov eax, ebp
    add eax, -8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, va
    push eax
    mov eax, ebp
    add eax, -4
    push dword [eax]
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, va
    push eax
    mov eax, ebp
    add eax, -8
    push dword [eax]
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jg .L2
    push 0
    jmp .L3
    .L2: 
    push 1
    .L3: 
    pop eax
    test eax, eax
    jz .L1
    sub esp, 8
    push format0
    call _printf
    add esp, 12
    .L1: 
    add esp, 16
    pop ebp
    ret 8

_main:
    mov eax, depth
    mov [eax], ebp
    push 1
    mov eax, vmax
    push dword [eax]
    call c2sort140268183823368
    push 1
    mov eax, vi
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L1: 
    mov eax, vi
    push dword [eax]
    push 20
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
    push format1
    call _printf
    add esp, 12
    mov eax, vi
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, vi
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    jmp .L1
    .L2: 
    mov eax, 0
    ret
