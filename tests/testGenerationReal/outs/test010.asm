bits 32
extern _printf
global _main
section .data
    depth: times 0 dd 0
    format0 : db '%f', 10, 0
    va: dq 500.01
    vb: dd 10
    d1: dq 0.1
    d2: dq 1.0
    d3: dq 5.00001
    d4: dq 1.0
    d5: dq 5.00001
section .text
_main:
    mov eax, va
    push dword [eax + 4]
    push dword [eax]
    push dword [d1 + 4]
    push dword [d1]
    fld qword [esp]
    add esp, 8
    fadd qword [esp]
    fstp qword [esp]
    mov eax, va
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    pop ebx
    mov [eax + 4], ebx
    mov eax, vb
    push dword [eax]
    mov eax, va
    push dword [eax + 4]
    push dword [eax]
    fld qword [esp]
    add esp, 8
    fiadd dword [esp]
    sub esp, 4
    fstp qword [esp]
    mov eax, va
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    pop ebx
    mov [eax + 4], ebx
    push dword [d2 + 4]
    push dword [d2]
    mov eax, va
    push dword [eax + 4]
    push dword [eax]
    fld qword [esp]
    add esp, 8
    fmul qword [esp]
    fstp qword [esp]
    mov eax, va
    push eax
    pop eax
    pop ebx
    mov [eax], ebx
    pop ebx
    mov [eax + 4], ebx
    sub esp, 0
    push dword [d3 + 4]
    push dword [d3]
    push dword [d4 + 4]
    push dword [d4]
    mov eax, va
    push dword [eax + 4]
    push dword [eax]
    fld qword [esp]
    add esp, 8
    fmul qword [esp]
    fstp qword [esp]
    push dword [d5 + 4]
    push dword [d5]
    fld qword [esp]
    add esp, 8
    fadd qword [esp]
    fstp qword [esp]
    fld qword [esp]
    add esp, 8
    fsub qword [esp]
    fstp qword [esp]
    push format0
    call _printf
    add esp, 12
    mov eax, 0
    ret
