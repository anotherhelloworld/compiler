bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    va: dd 10
    vb: dd 0
section .text
_main:
    mov eax, va
    push dword [eax]
    mov eax, vb
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    sub esp, 4
    mov eax, vb
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
