;6
;6
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%s', 10, 0
    str0: db 'Hello world', 0
    va: db 0
section .text
_main:
    .L1: 
    push 1
    mov eax, va
    pop ebx
    mov [eax], ebx
    sub esp, 4
    push str0
    push format0
    call _printf
    add esp, 12
    .L3: 
    mov eax, 1
    test eax, eax
    jz .L6
    push 1
    jmp .L7
    .L6: 
    push eax
    .L7: 
    pop eax
    test eax, eax
    jnz .L4
    mov eax, 1
    test eax, eax
    jz .L8
    push 1
    jmp .L9
    .L8: 
    push eax
    .L9: 
    jmp .L5
    .L4: 
    push eax
    .L5: 
    pop eax
    test eax, eax
    jz .L1
    .L2: 
    mov eax, 0
    ret
