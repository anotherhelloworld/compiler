bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
section .text
_main:
    mov eax, 0
    ret
