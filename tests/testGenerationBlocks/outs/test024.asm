bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
    vmax: dd 20
    vmin: dd 1
    va: times 20 dd 20, 10, 11, 12, 13, 19, 18, 17, 16, 15, 14, 1, 2, 3, 6, 4, 5, 7, 9, 8
    vi: dd 0
section .text
c3quicksort140559939449880:
    push ebp
    mov ebp, esp
    sub esp, 20
    mov eax, depth
    mov [eax + 4], ebp
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, 8
    push dword [eax]
    call c2sort140559939450152
    add esp, 20
    pop ebp
    ret 12
c2sort140559939450152:
    push ebp
    mov ebp, esp
    sub esp, 16
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, ebp
    add eax, 8
    push dword [eax]
    mov eax, ebp
    add eax, -8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
    push eax
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, 8
    push dword [eax]
    pop ebx
    pop eax
    add eax, ebx
    push eax
    push 2
    pop ebx
    pop eax
    xor edx, edx
    div ebx
    push eax
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, ebp
    add eax, -12
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L1: 
    .L4: 
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
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
    mov eax, ebp
    add eax, -12
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jl .L6
    push 0
    jmp .L7
    .L6: 
    push 1
    .L7: 
    pop eax
    test eax, eax
    jz .L5
    mov eax, ebp
    add eax, -4
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, ebp
    add eax, -4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    jmp .L4
    .L5: 
    .L8: 
    mov eax, ebp
    add eax, -12
    push dword [eax]
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
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
    jl .L10
    push 0
    jmp .L11
    .L10: 
    push 1
    .L11: 
    pop eax
    test eax, eax
    jz .L9
    mov eax, ebp
    add eax, -8
    push dword [eax]
    push 1
    pop ebx
    pop eax
    sub eax, ebx
    push eax
    mov eax, ebp
    add eax, -8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    jmp .L8
    .L9: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov eax, ebp
    add eax, -8
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jle .L13
    push 0
    jmp .L14
    .L13: 
    push 1
    .L14: 
    pop eax
    test eax, eax
    jz .L12
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
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
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
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
    jg .L16
    push 0
    jmp .L17
    .L16: 
    push 1
    .L17: 
    pop eax
    test eax, eax
    jz .L15
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
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
    mov eax, ebp
    add eax, -16
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
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
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
    push eax
    mov eax, ebp
    add eax, -4
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
    mov eax, ebp
    add eax, -16
    push dword [eax]
    mov eax, [depth + 4]
    add eax, 16
    mov eax, [eax]
    push eax
    mov eax, ebp
    add eax, -8
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
    .L15: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, ebp
    add eax, -4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, ebp
    add eax, -8
    push dword [eax]
    push 1
    pop ebx
    pop eax
    sub eax, ebx
    push eax
    mov eax, ebp
    add eax, -8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L12: 
    .L3: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov eax, ebp
    add eax, -8
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jge .L18
    push 0
    jmp .L19
    .L18: 
    push 1
    .L19: 
    pop eax
    test eax, eax
    jz .L1
    .L2: 
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -8
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jl .L21
    push 0
    jmp .L22
    .L21: 
    push 1
    .L22: 
    pop eax
    test eax, eax
    jz .L20
    mov eax, depth
    mov [eax + 8], ebp
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -8
    push dword [eax]
    call c2sort140559939450152
    .L20: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov eax, ebp
    add eax, 8
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jl .L24
    push 0
    jmp .L25
    .L24: 
    push 1
    .L25: 
    pop eax
    test eax, eax
    jz .L23
    mov eax, depth
    mov [eax + 8], ebp
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov eax, ebp
    add eax, 8
    push dword [eax]
    call c2sort140559939450152
    .L23: 
    add esp, 16
    pop ebp
    ret 8


_main:
    mov eax, depth
    mov [eax], ebp
    mov eax, va
    push eax
    mov eax, vmin
    push dword [eax]
    mov eax, vmax
    push dword [eax]
    call c3quicksort140559939449880
    mov eax, vmin
    push dword [eax]
    mov eax, vi
    push eax
    pop eax
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
