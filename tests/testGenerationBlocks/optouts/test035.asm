bits 32
extern _printf
global _main
section .data
    depth: times 4 dd 0
    format0 : db '%s', 10, 0
    str0: db 'Hello world!', 0
section .text
c1A140535671362648:
    push ebp
    mov ebp, esp
    sub esp, 12
    mov eax, depth
    mov [eax + 4], ebp
    mov eax, ebp
    add eax, 8
    push dword [eax]
    call c1c140535671362856
    add esp, 12
    pop ebp
    ret 4
c1c140535671362856:
    push ebp
    mov ebp, esp
    sub esp, 12
    sub esp, 4
    push str0
    push format0
    call _printf
    add esp, 12
    add esp, 12
    pop ebp
    ret 4


_main:
    mov eax, depth
    mov [eax], ebp
    push 10
    call c1A140535671362648
    mov eax, 0
    ret
