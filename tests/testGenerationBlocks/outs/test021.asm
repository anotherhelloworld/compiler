bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '', 10, 0
    format1 : db '%s', 10, 0
    str0: db 'Hello world!', 0
    va: dd 20
    vb: dd 10
section .text
c2P140347633359416:
    push ebp
    mov ebp, esp
    sub esp, 16
    mov eax, ebp
    add eax, 12
    mov eax, [eax]
    push dword [eax]
    mov eax, ebp
    add eax, 8
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
    sub esp, 4
    push str0
    push format1
    call _printf
    add esp, 12
    add esp, 16
    pop ebp
    ret 8

_main:
    mov eax, depth
    mov [eax], ebp
    mov eax, vb
    push eax
    mov eax, va
    push dword [eax]
    call c2P140347633359416
    mov eax, 0
    ret
