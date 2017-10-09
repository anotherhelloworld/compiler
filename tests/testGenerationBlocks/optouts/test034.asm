;6
;10
;2
;10
bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
section .text
c3A140543559707288:
    push ebp
    mov ebp, esp
    sub esp, 16
    push 10
    mov eax, ebp
    add eax, -4
    pop ebx
    mov [eax], ebx
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov ebx, 10
    pop eax
    imul eax, ebx
    mov ebx, eax
    add [esp], ebx
    mov eax, ebp
    add eax, 8
    push dword [eax]
    pop ebx
    sub [esp], ebx
    mov eax, ebp
    add eax, 16
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
    call c3A140543559707288
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
