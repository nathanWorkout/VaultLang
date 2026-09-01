#include <string.h>
#include "../include/parser.h"
#include "../include/codegen.h"

Codegen codegen_new(FILE *asm_file) {
    Codegen cg;
    cg.asm_file = asm_file;
    cg.stack_offset = 0;
    return cg;
}

void codegen_prologue(Codegen *cg) {
    fprintf(cg->asm_file, "section .data\n");
    fprintf(cg->asm_file, "section .bss\n");
    fprintf(cg->asm_file, "section .text\n\n");
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
    if (strcmp(node->var_type, "i8") == 0) {
        cg->stack_offset += 1;
        fprintf(cg->asm_file, "%%define %s rbp - %d\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov byte %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "i16") == 0) {
        // alignement sinon ca donne un offset faut dans la stack
        cg->stack_offset = (cg->stack_offset + 1) & ~1;
        cg->stack_offset += 2;
        fprintf(cg->asm_file, "%%define %s rbp - %d\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov word %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "i32") == 0) {
        cg->stack_offset = (cg->stack_offset + 3) & ~3;
        cg->stack_offset += 4;
        fprintf(cg->asm_file, "%%define %s rbp - %d\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov dword %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "i64") == 0) {
        cg->stack_offset = (cg->stack_offset + 7) & ~7;
        cg->stack_offset += 8;
        fprintf(cg->asm_file, "%%define %s rbp - %d\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov qword %s, %s\n", node->name, node->value);
    }
}
