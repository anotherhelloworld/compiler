bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d%c%d', 0
section .text
_main:
    sub esp, 12
    push 20
    push ' '
    push 10
    push format0
    call _printf
    add esp, 28
    mov eax, 0
    ret
