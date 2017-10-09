bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d', 10, 0
section .text
c2A140641543896712:
    push ebp
    mov ebp, esp
    sub esp, 12
    mov eax, ebp
    add eax, 8
    push dword [eax]
    push 10
    pop ebx
    pop eax
    add eax, ebx
    push eax
    mov eax, ebp
    add eax, 12
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    add esp, 12
    pop ebp
    ret 4

_main:
    sub esp, 8
    mov eax, depth
    mov [eax], ebp
    push 5
    call c2A140641543896712
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
