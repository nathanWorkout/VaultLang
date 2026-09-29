section .data
    str0 db "", 10, 0

section .bss
    int_buf resb 21
section .text

global _start

_start:
    jmp _main

itoa:
    push rbx
    push r12
    push r13

    xor rbx, rbx
    test rax, rax
    jns .positive
    neg rax
    mov byte [int_buf], '-'
    mov rbx, 1

.positive:
    mov r12, 10
    xor rcx, rcx

.loop_itoa:
    xor rdx, rdx
    div r12
    add rdx, 48
    push rdx
    inc rcx
    test rax, rax
    jnz .loop_itoa
    lea r13, [rcx + rbx]
    lea rsi, [int_buf + rbx]

.depile:
    pop rdx
    mov byte [rsi], dl
    inc rsi
    loop .depile
    mov rax, 1
    mov rdi, 1
    mov rsi, int_buf
    mov rdx, r13
    syscall
    pop r13
    pop r12
    pop rbx
    ret


_main:
    push rbp
    mov rbp, rsp
    sub rsp, 16

%define negative [rbp - 1]
    mov byte negative, -34
%define positive [rbp - 16]
    mov qword positive, 40567
    movsx rax, byte [rbp - 1]
    call itoa
    mov rax, 1
    mov rdi, 1
    mov rsi, str0
    mov rdx, 1
    syscall
    mov rax, [rbp - 16]
    call itoa

    mov rsp, rbp
    pop rbp
    mov rax, 60
    mov rdi, 0
    syscall
