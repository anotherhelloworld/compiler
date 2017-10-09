;2
;5
;6
;7
;10
;5
;6
;10
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d%c%d', 0
section .text
_main:
    sub esp, 12
    mov eax, 10
    imul eax, 20
    push eax
    mov ebx, 99
    pop eax
    xor edx, edx
    div ebx
    push edx
    push ' '
    push 10
    mov ebx, 20
    add [esp], ebx
    mov eax, 30
    mov ebx, 30
    xor edx, edx
    div ebx
    mov ebx, eax
    add [esp], ebx
    push format0
    call _printf
    add esp, 28
    mov eax, 0
    ret
