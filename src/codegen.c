#include <string.h>
#include "../include/codegen.h"

Codegen codegen_new(FILE *asm_file) {
    Codegen cg;
    cg.asm_file = asm_file;
    cg.stack_offset = 0;
    return cg;
}

void codegen_prologue(Codegen *cg) {
    fprintf(cg->asm_file, "section .text\n");
    fprintf(cg->asm_file, "global _start\n");
    fprintf(cg->asm_file, "\n_start:\n");
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

void codegen_var_decl(Codegen *cg, Node *node) {
    if (strcmp(node->var_type, "i32") == 0) {
        // alignement sinon ca fait nimporte quoi au niveau de l'aligneent de la pile [rbp - _]
        cg->stack_offset = (cg->stack_offset + 3) & ~3;
        cg->stack_offset += 4;
        fprintf(cg->asm_file, "    mov dword [rbp - %d], %s\n",
                cg->stack_offset, node->value);
    }
    else if (strcmp(node->var_type, "i64") == 0) {
        cg->stack_offset = (cg->stack_offset + 7) & ~7;
        cg->stack_offset += 8;
        fprintf(cg->asm_file, "    mov qword [rbp - %d], %s\n",
                cg->stack_offset, node->value);
    }
}