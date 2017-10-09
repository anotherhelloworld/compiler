;5
;6
;7
;10
;6
;10
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 0
section .text
_main:
    sub esp, 4
    mov eax, 10
    add eax, 20
    push eax
    mov ebx, 30
    add [esp], ebx
    mov ebx, 40
    add [esp], ebx
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
