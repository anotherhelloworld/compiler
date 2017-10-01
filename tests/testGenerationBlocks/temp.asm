bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    vm: dd 20
    varr: times 20 dd 20, 10, 11, 12, 13, 19, 18, 17, 16, 15, 14, 1, 2, 3, 6, 4, 5, 7, 9, 8
    vi: dd 0
    vj: dd 0
    vk: dd 0
section .text
_main:
    push 1
    mov eax, vi
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L1: 
    mov eax, vi
    push dword [eax]
    mov eax, vm
    push dword [eax]
    push 1
    pop ebx
    pop eax
    sub eax, ebx
    push eax
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
    push 1
    mov eax, vj
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L5: 
    mov eax, vj
    push dword [eax]
    mov eax, vm
    push dword [eax]
    mov eax, vi
    push dword [eax]
    pop ebx
    pop eax
    sub eax, ebx
    push eax
    pop ebx
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
    mov eax, varr
    push eax
    mov eax, vj
    push dword [eax]
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, varr
    push eax
    mov eax, vj
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jg .L10
    push 0
    jmp .L11
    .L10: 
    push 1
    .L11: 
    pop eax
    test eax, eax
    jz .L9
    mov eax, varr
    push eax
    mov eax, vj
    push dword [eax]
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, vk
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, varr
    push eax
    mov eax, vj
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, varr
    push eax
    mov eax, vj
    push dword [eax]
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, vk
    push dword [eax]
    mov eax, varr
    push eax
    mov eax, vj
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L9: 
    mov eax, vj
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, vj
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    jmp .L5
    .L6: 
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
    push 1
    mov eax, vi
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L12: 
    mov eax, vi
    push dword [eax]
    mov eax, vm
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jg .L14
    push 0
    jmp .L15
    .L14: 
    push 1
    .L15: 
    pop eax
    test eax, eax
    jnz .L13
    sub esp, 4
    mov eax, varr
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
    jmp .L12
    .L13: 
    mov eax, 0
    ret
