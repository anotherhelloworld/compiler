bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d%c%d%c%d', 10, 0
    format1 : db '', 10, 0
    format2 : db '%d%c%d%c%d', 0
section .text
_main:
    sub esp, 4
    push 3
    push ' '
    push 2
    push ' '
    push 1
    push format0
    call _printf
    add esp, 28
    sub esp, 8
    push format1
    call _printf
    add esp, 12
    sub esp, 4
    push 6
    push ' '
    push 5
    push ' '
    push 4
    push format2
    call _printf
    add esp, 28
    mov eax, 0
    ret
