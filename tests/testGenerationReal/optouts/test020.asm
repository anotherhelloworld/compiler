;8
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 10, 0
    va: dq 10.0
    d1: dq 10.0
    d2: dq 100.5
section .text
_main:
    mov eax, va
    push dword [eax + 4]
    push dword [eax]
    push dword [d1 + 4]
    push dword [d1]
    fld qword [esp]
    fld qword [esp + 8]
    add esp, 16
    fcomip ST0, ST1
    jbe .L2
    push 0
    jmp .L3
    .L2: 
    push 1
    .L3: 
    pop eax
    test eax, eax
    jz .L1
    push dword [d2 + 4]
    push dword [d2]
    push format0
    call _printf
    add esp, 12
    .L1: 
    mov eax, 0
    ret
