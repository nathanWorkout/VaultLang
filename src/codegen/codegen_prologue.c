#include <stdio.h>
#include "../../include/codegen.h"

void codegen_prologue(Codegen *cg) {
    fprintf(cg->asm_file, "section .data\n");
    fprintf(cg->asm_file, "%s\n", cg->data_section); // on vide le buffer des strings
    fprintf(cg->asm_file, "section .bss\n");
    fprintf(cg->asm_file, "    int_buf resb 20\n");  // buffer pour print des int (20 caracteres. a augmenter plus tard)
    fprintf(cg->asm_file, "section .text\n\n");
    fprintf(cg->asm_file, "global _start\n");
    fprintf(cg->asm_file, "\n_start:\n");
    fprintf(cg->asm_file, "    jmp _main\n\n");

    // routine itoa elle recoit un entier et le print
    fprintf(cg->asm_file, "itoa:\n");
    fprintf(cg->asm_file, "    mov r10, 10\n");
    fprintf(cg->asm_file, "    xor rcx, rcx\n\n");
    fprintf(cg->asm_file, ".loop_itoa:\n");
    fprintf(cg->asm_file, "    xor rdx, rdx\n");
    fprintf(cg->asm_file, "    div r10\n");
    fprintf(cg->asm_file, "    add rdx, 48\n");
    fprintf(cg->asm_file, "    push rdx\n");
    fprintf(cg->asm_file, "    inc rcx\n");
    fprintf(cg->asm_file, "    test rax, rax\n");
    fprintf(cg->asm_file, "    jnz .loop_itoa\n");
    fprintf(cg->asm_file, "    mov r11, rcx\n");
    fprintf(cg->asm_file, "    mov rsi, int_buf\n\n");
    fprintf(cg->asm_file, ".depile:\n");
    fprintf(cg->asm_file, "    pop rdx\n");
    fprintf(cg->asm_file, "    mov byte [rsi], dl\n");
    fprintf(cg->asm_file, "    inc rsi\n");
    fprintf(cg->asm_file, "    loop .depile\n");
    fprintf(cg->asm_file, "    mov rax, 1\n");
    fprintf(cg->asm_file, "    mov rdi, 1\n");
    fprintf(cg->asm_file, "    mov rsi, int_buf\n");
    fprintf(cg->asm_file, "    mov rdx, r11\n");
    fprintf(cg->asm_file, "    syscall\n");
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