bits 32
extern _printf
global _main
section .data
    format0 : db '%f', 0
    d1: dq 10.0
    d2: dq 20.0
section .text
_main:
    sub esp, 0
    push dword [d1 + 4]
    push dword [d1]
    push dword [d2 + 4]
    push dword [d2]
    fld qword [esp]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
