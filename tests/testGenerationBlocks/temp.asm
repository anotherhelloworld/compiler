bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    format1 : db '%d', 10, 0
    format2 : db '%d', 10, 0
    va: times 4 db 0
section .text
_main:
    push 10
    mov eax, va
    add eax, 8
    pop ebx
    mov [eax], ebx
    mov eax, 2
    mov ebx, 2
    imul eax, ebx
    push eax
    mov eax, va
    add eax, 0
    pop ebx
    mov [eax], ebx
    push 3
    mov ebx, 3
    pop eax
    imul eax, ebx
    push eax
    mov eax, va
    add eax, 4
    pop ebx
    mov [eax], ebx
    sub esp, 4
    mov eax, va
    add eax, 0
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    sub esp, 4
    mov eax, va
    add eax, 4
    push dword [eax]
    push format1
    call _printf
    add esp, 12
    sub esp, 4
    mov eax, va
    add eax, 8
    push dword [eax]
    push format2
    call _printf
    add esp, 12
    mov eax, 0
    ret
