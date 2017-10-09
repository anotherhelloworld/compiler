bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 10, 0
    d1: dq 20.0
section .text
_main:
    sub esp, 0
    push dword [d1 + 4]
    push dword [d1]
    push 10
    fild dword [esp]
    add esp, 4
    fmul qword [esp]
    fstp qword [esp]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
