bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 10, 0
    va: dq 0.0
    d1: dq 10.1
section .text
_main:
    push dword [d1 + 4]
    push dword [d1]
    mov eax, va
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    pop ebx
    mov [eax + 4], ebx
    sub esp, 0
    mov eax, va
    push dword [eax + 4]
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
