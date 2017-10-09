bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
    vmax: dd 20
    va: times 20 dd 20, 10, 11, 12, 13, 19, 18, 17, 16, 15, 14, 1, 2, 3, 6, 4, 5, 7, 9, 8
    vi: dd 0
    vvalue: dd 0
section .text
c3quicksort140185472145352:
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
    call c3quicksort140185472145352
    mov eax, va
    push eax
    push 1
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, vvalue
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    push 1
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
    mov eax, va
    push eax
    mov eax, vi
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
    mov eax, va
    push eax
    mov eax, vi
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
    mov eax, va
    push eax
    push 1
    pop ebx
    sub ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    mov eax, va
    push eax
    mov eax, vmax
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
    push 1
    mov eax, vi
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L5: 
    mov eax, vi
    push dword [eax]
    mov eax, vmax
    push dword [eax]
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
    jmp .L5
    .L6: 
    mov eax, 0
    ret
