default rel

section .data
    hello1 db "Entrez le premier nombre : ", 0
    hello2 db "Entrez le  2eme nombre : ", 0

section .bss
    buffer1 resb 8
    buffer2 resb 8
    buffer3 resb 20 ; max de nombres pour 64 bits
section .text


global _start

_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, hello1
    mov rdx, 27
    syscall

    mov rax, 0
    mov rdi, 0
    mov rsi, buffer1
    mov rdx, 8
    syscall

    call atoi
    mov r8, rax

    mov rax, 1
    mov rdi, 1
    mov rsi, hello2
    mov rdx, 25
    syscall

    mov rax, 0
    mov rdi, 0
    mov rsi, buffer2
    mov rdx, 8
    syscall

    call atoi
    mov r9, rax

    add r8, r9
    call itoa
    
    mov rax, 60
    mov rdi, 0
    syscall

atoi:
    xor rax, rax

.loop:
    movzx r10, byte[rsi] ; comme l->current_char
    cmp r10, 10 ; \n
    je .done

    cmp r10, 0 ; \0
    je .done

    sub r10, 48 ; A = 48 en asci
    imul rax, 10
    add rax, r10
    inc rsi
    jmp .loop

.done:
    ret

itoa:
    mov rax, r8
    mov r10, 10
    xor rcx, rcx

.loop:
    xor rdx, rdx
    div r10 ; divise automatiquement par rax, reste dans rdx
    add rdx, 48 
    push rdx 
    inc rcx  
    test rax, rax
    jnz .loop

    mov r11, rcx
    mov rsi, buffer3

.depile:
    pop rdx
    mov byte[rsi], dl
    inc rsi
    loop .depile

    mov rax, 1
    mov rdi, 1
    mov rsi, buffer3
    mov rdx, r11
    syscall
    ret


