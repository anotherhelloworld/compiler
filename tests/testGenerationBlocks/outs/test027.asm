bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    format1 : db '%d', 10, 0
    format2 : db '%d', 10, 0
    va: dd 0
section .text
_main:
    push 16
    mov eax, va
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    push 10
    mov eax, va
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    je .L2
    push 0
    jmp .L3
    .L2: 
    push 1
    .L3: 
    pop eax
    test eax, eax
    jz .L4
    sub esp, 4
    push 10
    push format0
    call _printf
    add esp, 12
    jmp .L1
    .L4: 
    push 13
    mov eax, va
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    je .L5
    push 0
    jmp .L6
    .L5: 
    push 1
    .L6: 
    pop eax
    test eax, eax
    jz .L7
    sub esp, 4
    push 20
    push format1
    call _printf
    add esp, 12
    jmp .L1
    .L7: 
    mov eax, va
    push dword [eax]
    push 15
    pop ebx
    pop eax
    cmp eax, ebx
    jge .L8
    push 0
    jmp .L9
    .L8: 
    push 1
    .L9: 
    mov eax, va
    push dword [eax]
    push 30
    pop ebx
    pop eax
    cmp eax, ebx
    jle .L10
    push 0
    jmp .L11
    .L10: 
    push 1
    .L11: 
    pop eax
    pop ebx
    and eax, eax
    test eax, eax
    jz .L12
    sub esp, 4
    push 30
    push format2
    call _printf
    add esp, 12
    jmp .L1
    .L12: 
    .L1: 
    mov eax, 0
    ret
