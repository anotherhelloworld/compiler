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
    push eax
    pop eax
    add eax, 8
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    push 2
    push 2
    pop ebx
    pop eax
    imul eax, ebx
    push eax
    mov eax, va
    push eax
    pop eax
    add eax, 0
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    push 3
    push 3
    pop ebx
    pop eax
    imul eax, ebx
    push eax
    mov eax, va
    push eax
    pop eax
    add eax, 4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    sub esp, 4
    mov eax, va
    push eax
    pop eax
    add eax, 0
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    sub esp, 4
    mov eax, va
    push eax
    pop eax
    add eax, 4
    push dword [eax]
    push format1
    call _printf
    add esp, 12
    sub esp, 4
    mov eax, va
    push eax
    pop eax
    add eax, 8
    push dword [eax]
    push format2
    call _printf
    add esp, 12
    mov eax, 0
    ret
