;8
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 0
    d1: dq 10.0
section .text
_main:
    push dword [d1 + 4]
    push dword [d1]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
