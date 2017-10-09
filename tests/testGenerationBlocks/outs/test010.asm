bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d%c%d', 10, 0
section .text
c4A140498193110264:
    push ebp
    mov ebp, esp
    sub esp, 24
    mov eax, ebp
    add eax, 20
    push dword [eax]
    mov eax, ebp
    add eax, -4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L1: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov eax, ebp
    add eax, 16
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
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    .L5: 
    mov eax, ebp
    add eax, -8
    push dword [eax]
    mov eax, ebp
    add eax, 8
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
    sub esp, 12
    mov eax, ebp
    add eax, -8
    push dword [eax]
    push ' '
    mov eax, ebp
    add eax, -4
    push dword [eax]
    push format0
    call _printf
    add esp, 28
    mov eax, ebp
    add eax, -8
    push dword [eax]
    push 1
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, ebp
    add eax, -8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    jmp .L5
    .L6: 
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
    jmp .L1
    .L2: 
    add esp, 24
    pop ebp
    ret 16

_main:
    mov eax, depth
    mov [eax], ebp
    push 1
    push 4
    push 1
    push 4
    call c4A140498193110264
    mov eax, 0
    ret
