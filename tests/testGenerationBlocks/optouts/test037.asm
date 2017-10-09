;6
;8
;6
;8
bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%d', 10, 0
    va: times 10 dd 0
section .text
_main:
    push 5
    mov eax, va
    push eax
    mov ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    pop ebx
    mov [eax], ebx
    sub esp, 4
    mov eax, va
    push eax
    mov ebx, 1
    imul ebx, 4
    pop eax
    add eax, ebx
    push dword [eax]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
