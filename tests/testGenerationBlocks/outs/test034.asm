bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
section .text
c3A140645688995064:
    push ebp
    mov ebp, esp
    sub esp, 16
    push 10
    mov eax, ebp
    add eax, -4
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -4
    push dword [eax]
    push 10
    pop ebx
    pop eax
    imul eax, ebx
    push eax
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, ebp
    add eax, 8
    push dword [eax]
    pop ebx
    pop eax
    sub eax, ebx
    push eax
    mov eax, ebp
    add eax, 16
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    add esp, 16
    pop ebp
    ret 8

_main:
    sub esp, 8
    mov eax, depth
    mov [eax], ebp
    push 5
    push 6
    call c3A140645688995064
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
