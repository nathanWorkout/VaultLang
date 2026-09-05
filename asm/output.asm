section .data
    str0 db "", 10, "Salut", 10, 0
    str1 db "Yoo", 0

section .bss
section .text

global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

%define age2 [rbp - 4]
    mov dword age2, 18
%define age3 [rbp - 16]
    mov qword age3, 89
    mov rax, 1
    mov rdi, 1
    mov rsi, str0
    mov rdx, 7
    syscall
    mov rax, 1
    mov rdi, 1
    mov rsi, str1
    mov rdx, 3
    syscall

    mov rsp, rbp
    pop rbp
    mov rax, 60
    mov rdi, 0
    syscall
