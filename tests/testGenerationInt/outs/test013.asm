bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%c%c%c%c%c%c%c%c%c%c%c%c', 10, 0
section .text
_main:
    sub esp, 8
    push '!'
    push 'd'
    push 'l'
    push 'r'
    push 'o'
    push 'w'
    push ' '
    push 'o'
    push 'l'
    push 'l'
    push 'e'
    push 'h'
    push format0
    call _printf
    add esp, 60
    mov eax, 0
    ret
