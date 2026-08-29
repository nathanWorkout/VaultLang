section .data
    msg db "hello", 0

section .text
global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    mov rax, msg
    mov qword [rbp - 8], rax  ; stocke l'adresse de msg dans la variable

    mov rax, 1
    mov rdi, 1
    mov rsi, [rbp - 8]     
    mov rdx, 5
    syscall

    mov rsp, rbp
    pop rbp

    mov rax, 60
    mov rdi, 0
    syscall