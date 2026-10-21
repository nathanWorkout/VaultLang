section .data
    str0 db "", 10, "salut", 10, 0

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


_udiv:
    pop rcx
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rax
    push rcx
    ret

_add:
    pop rcx
    pop rbx
    pop rax
    add rax, rbx
    push rax
    push rcx
    ret

_sub:
    pop rcx
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    push rcx
    ret

_mul:
    pop rcx
    pop rbx
    pop rax
    imul rax, rbx
    push rax
    push rcx
    ret

_div:
    pop rcx
    pop rbx
    pop rax
    cqo
    idiv rbx
    push rax
    push rcx
    ret

_main:
    push rbp
    mov rbp, rsp
    sub rsp, 128

%define x [rbp - 4]
    mov rax, 5
    push rax
    mov rax, 5
    push rax
    call _mul
    pop rax
    push rax
    mov rax, 10
    push rax
    call _add
    pop rax
    push rax
    mov rax, 5
    push rax
    call _sub
    pop rax
    mov dword x, eax
    mov rax, 1
    mov rdi, 1
    mov rsi, str0
    mov rdx, 7
    syscall
    mov eax, dword [rbp - 4]
    xor rdx, rdx
    call itoa

    mov rsp, rbp
    pop rbp
    mov rax, 60
    mov rdi, 0
    syscall
