;8
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 10, 0
    d1: dq 1.0
    d2: dq 2.0
    d3: dq 10.0
section .text
_main:
    push dword [d1 + 4]
    push dword [d1]
    push dword [d2 + 4]
    push dword [d2]
    fld qword [esp]
    add esp, 8
    fdiv qword [esp]
    fstp qword [esp]
    push 2
    push dword [d3 + 4]
    push dword [d3]
    fld qword [esp]
    add esp, 8
    fidiv dword [esp]
    sub esp, 4
    fstp qword [esp]
    fld qword [esp]
    add esp, 8
    fsub qword [esp]
    fstp qword [esp]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
