bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d%c%d', 10, 0
    format1 : db '%d%c%d', 10, 0
section .text
_main:
    sub esp, 12
    push 20
    push ' '
    push 10
    push format0
    call _printf
    add esp, 28
    sub esp, 12
    push 25
    push ' '
    push 15
    push format1
    call _printf
    add esp, 28
    mov eax, 0
    ret
