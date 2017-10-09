;6
;10
;6
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    format1 : db '%s', 10, 0
    str0: db 'letgetit', 0
    va: dd 10
section .text
_main:
    .L1: 
    sub esp, 4
    mov eax, va
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    mov eax, va
    push dword [eax]
    mov ebx, 1
    sub [esp], ebx
    mov eax, va
    pop ebx
    mov [eax], ebx
    .L3: 
    mov eax, va
    push dword [eax]
    mov ebx, 0
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
    sub esp, 4
    push str0
    push format1
    call _printf
    add esp, 12
    mov eax, 0
    ret
