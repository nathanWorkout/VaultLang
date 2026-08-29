section .text
global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    mov dword [rbp - 4], 18
    mov qword [rbp - 16], 89

    mov rsp, rbp
    pop rbp
    mov rax, 60
    mov rdi, 0
    syscall
