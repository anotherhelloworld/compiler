bits 32
extern _printf
global _main
section .data
    format0 : db '%d', 10, 0
    vi: dd 0
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
    push 5
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
    .L5: 
    push 1
    pop eax
    test eax, eax
    jz .L6
    sub esp, 4
    mov eax, vi
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    jmp .L6
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
    mov eax, 0
    ret
