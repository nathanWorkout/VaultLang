#include <stdio.h>
#include "../../include/codegen.h"

void codegen_prologue(Codegen *cg) {
    fprintf(cg->asm_file, "section .data\n");
    fprintf(cg->asm_file, "%s\n", cg->data_section); // on vide le buffer des strings
    fprintf(cg->asm_file, "section .bss\n");
    fprintf(cg->asm_file, "    int_buf resb 21\n");  // buffer pour print des int (21 caracteres. a augmenter plus tard)
    fprintf(cg->asm_file, "section .text\n\n");
    fprintf(cg->asm_file, "global _start\n");
    fprintf(cg->asm_file, "\n_start:\n");
    fprintf(cg->asm_file, "    jmp _main\n\n");

    // routine itoa elle recoit un entier et le print
    fprintf(cg->asm_file, "itoa:\n");
    fprintf(cg->asm_file, "    push rbx\n");
    fprintf(cg->asm_file, "    push r12\n");
    fprintf(cg->asm_file, "    push r13\n\n");
    fprintf(cg->asm_file, "    xor rbx, rbx\n");              // r8 = 0 (pas de signe)
    fprintf(cg->asm_file, "    test rax, rax\n");
    fprintf(cg->asm_file, "    jns .positive\n");
    fprintf(cg->asm_file, "    neg rax\n");
    fprintf(cg->asm_file, "    mov byte [int_buf], '-'\n"); // met le - dans le int buffer
    fprintf(cg->asm_file, "    mov rbx, 1\n\n");             // si r8 = 1 : signe présent
    fprintf(cg->asm_file, ".positive:\n");
    fprintf(cg->asm_file, "    mov r12, 10\n");
    fprintf(cg->asm_file, "    xor rcx, rcx\n\n");
    fprintf(cg->asm_file, ".loop_itoa:\n");
    fprintf(cg->asm_file, "    xor rdx, rdx\n");
    fprintf(cg->asm_file, "    div r12\n");
    fprintf(cg->asm_file, "    add rdx, 48\n");
    fprintf(cg->asm_file, "    push rdx\n");
    fprintf(cg->asm_file, "    inc rcx\n");
    fprintf(cg->asm_file, "    test rax, rax\n");
    fprintf(cg->asm_file, "    jnz .loop_itoa\n");
    fprintf(cg->asm_file, "    lea r13, [rcx + rbx]\n");     // longueur = nombre de chiffres + signe ex avec -34 : rcx = 2 et r8 = 1, r11 = 2 + 1 = 3 -> -34
    fprintf(cg->asm_file, "    lea rsi, [int_buf + rbx]\n\n"); // chiffres apres le -
    fprintf(cg->asm_file, ".depile:\n");
    fprintf(cg->asm_file, "    pop rdx\n");
    fprintf(cg->asm_file, "    mov byte [rsi], dl\n");
    fprintf(cg->asm_file, "    inc rsi\n");
    fprintf(cg->asm_file, "    loop .depile\n");
    fprintf(cg->asm_file, "    mov rax, 1\n");
    fprintf(cg->asm_file, "    mov rdi, 1\n");
    fprintf(cg->asm_file, "    mov rsi, int_buf\n");
    fprintf(cg->asm_file, "    mov rdx, r13\n");
    fprintf(cg->asm_file, "    syscall\n");
    fprintf(cg->asm_file, "    pop r13\n");
    fprintf(cg->asm_file, "    pop r12\n");
    fprintf(cg->asm_file, "    pop rbx\n");
    fprintf(cg->asm_file, "    ret\n\n\n");

    fprintf(cg->asm_file, "_main:\n");
    fprintf(cg->asm_file, "    push rbp\n");
    fprintf(cg->asm_file, "    mov rbp, rsp\n");
    fprintf(cg->asm_file, "    sub rsp, 16\n\n");
}

void codegen_epilogue(Codegen *cg) {
    fprintf(cg->asm_file, "\n    mov rsp, rbp\n");
    fprintf(cg->asm_file, "    pop rbp\n");
    fprintf(cg->asm_file, "    mov rax, 60\n");
    fprintf(cg->asm_file, "    mov rdi, 0\n");
    fprintf(cg->asm_file, "    syscall\n");
}
