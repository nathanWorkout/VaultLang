section .data

section .bss
    int_buf resb 20
section .text

global _start

_start:
    jmp _main

itoa:
    mov r10, 10
    xor rcx, rcx

.loop_itoa:
    xor rdx, rdx
    div r10
    add rdx, 48
    push rdx
    inc rcx
    test rax, rax
    jnz .loop_itoa
    mov r11, rcx
    mov rsi, int_buf

.depile:
    pop rdx
    mov byte [rsi], dl
    inc rsi
    loop .depile
    mov rax, 1
    mov rdi, 1
    mov rsi, int_buf
    mov rdx, r11
    syscall
    ret


_main:
    push rbp
    mov rbp, rsp
    sub rsp, 16

%define negative [rbp - 1]
    mov byte negative, -34
    movsx rax, byte [rbp - 1]
    call itoa

    mov rsp, rbp
    pop rbp
    mov rax, 60
    mov rdi, 0
    syscall
