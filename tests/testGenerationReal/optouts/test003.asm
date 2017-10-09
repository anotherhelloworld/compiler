;8
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 10, 0
    d1: dq 3.0000
    d2: dq 3.1111
section .text
_main:
    push 1
    push dword [d1 + 4]
    push dword [d1]
    push dword [d2 + 4]
    push dword [d2]
    fld qword [esp]
    add esp, 8
    fmul qword [esp]
    fstp qword [esp]
    fld qword [esp]
    add esp, 8
    fisub dword [esp]
    sub esp, 4
    fstp qword [esp]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
