section .data
section .bss
section .text

global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

%define age rbp - 1
    mov byte age, 3
%define age1 rbp - 4
    mov word age1, 17
