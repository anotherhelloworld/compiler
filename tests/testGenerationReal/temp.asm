bits 32
extern _printf
global _main
section .data
    format0 : db '%f', 10, 0
    va: dq 0.0
section .text
_main:
    push 10
    mov eax, va
    push eax
    pop eax
    fild dword [esp]
    sub esp, 4
    fstp qword [esp]
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
