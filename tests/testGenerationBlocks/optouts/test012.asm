;6
;10
;6
;10
bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%d%c%d', 10, 0
section .text
c2B140237724783800:
    push ebp
    mov ebp, esp
    sub esp, 16
    mov eax, depth
    mov [eax + 4], ebp
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, 8
    push dword [eax]
    call c2A140237724784088
    add esp, 16
    pop ebp
    ret 8
c2A140237724784088:
    push ebp
    mov ebp, esp
    sub esp, 16
    mov eax, [depth + 4]
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -4
    pop ebx
    mov [eax], ebx
    .L1: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov eax, ebp
    add eax, 8
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jg .L3
    push 0
    jmp .L4
    .L3: 
    push 1
    .L4: 
    pop eax
    test eax, eax
    jnz .L2
    mov eax, ebp
    add eax, 12
    push dword [eax]
    mov eax, ebp
    add eax, -8
    pop ebx
    mov [eax], ebx
    .L5: 
    mov eax, ebp
    add eax, -8
    push dword [eax]
    mov eax, [depth + 4]
    add eax, 8
    push dword [eax]
    pop ebx
    pop eax
    cmp eax, ebx
    jg .L7
    push 0
    jmp .L8
    .L7: 
    push 1
    .L8: 
    pop eax
    test eax, eax
    jnz .L6
    sub esp, 12
    mov eax, ebp
    add eax, -8
    push dword [eax]
    push ' '
    mov eax, ebp
    add eax, -4
    push dword [eax]
    push format0
    call _printf
    add esp, 28
    mov eax, ebp
    add eax, -8
    push dword [eax]
    mov ebx, 1
    add [esp], ebx
    mov eax, ebp
    add eax, -8
    pop ebx
    mov [eax], ebx
    jmp .L5
    .L6: 
    mov eax, ebp
    add eax, -4
    push dword [eax]
    mov ebx, 1
    add [esp], ebx
    mov eax, ebp
    add eax, -4
    pop ebx
    mov [eax], ebx
    jmp .L1
    .L2: 
    add esp, 16
    pop ebp
    ret 8


_main:
    mov eax, depth
    mov [eax], ebp
    push 1
    push 4
    call c2B140237724783800
    mov eax, 0
    ret
