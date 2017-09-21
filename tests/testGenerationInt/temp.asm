bits 32
extern _printf
global _main
section .data
    format0 : db '', 10, 0
    format1 : db '%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d', 0
section .text
_main:
    sub esp, 8
    push format0
    call _printf
    add esp, 12
    sub esp, 12
    push 105
    push ' '
    push 91
    push ' '
    push 78
    push ' '
    push 66
    push ' '
    push 55
    push ' '
    push 45
    push ' '
    push 36
    push ' '
    push 28
    push ' '
    push 21
    push ' '
    push 15
    push ' '
    push 10
    push ' '
    push 6
    push ' '
    push 3
    push ' '
    push 1
    push format1
    call _printf
    add esp, 124
    mov eax, 0
    ret
