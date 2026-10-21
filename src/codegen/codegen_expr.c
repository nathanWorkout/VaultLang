#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/codegen.h"

void codegen_expr(Codegen *cg, Node *node) {
    if (node->type == NODE_INT_LIT) {
        fprintf(cg->asm_file, "    mov rax, %s\n", node->value);
        return;
    }

    if (node->type == NODE_IDENT) {
        for (int i = 0; i < cg->var_table_count; i++) {
            if (strcmp(cg->var_table[i].name, node->name) == 0) {
                if (strcmp(cg->var_table[i].var_type, "i64") == 0)
                    fprintf(cg->asm_file, "    mov rax, [rbp - %d]\n", cg->var_table[i].stack_offset);
                else if (strcmp(cg->var_table[i].var_type, "i32") == 0)
                    fprintf(cg->asm_file, "    movsxd rax, dword [rbp - %d]\n", cg->var_table[i].stack_offset);
                else if (strcmp(cg->var_table[i].var_type, "i16") == 0)
                    fprintf(cg->asm_file, "    movsx rax, word [rbp - %d]\n", cg->var_table[i].stack_offset);
                else if (strcmp(cg->var_table[i].var_type, "i8") == 0)
                    fprintf(cg->asm_file, "    movsx rax, byte [rbp - %d]\n", cg->var_table[i].stack_offset);

                else if (strcmp(cg->var_table[i].var_type, "u64") == 0)
                    fprintf(cg->asm_file, "    mov rax, [rbp - %d]\n", cg->var_table[i].stack_offset);
                else if (strcmp(cg->var_table[i].var_type, "u32") == 0)
                    fprintf(cg->asm_file, "    mov eax, dword [rbp - %d]\n", cg->var_table[i].stack_offset);
                else if (strcmp(cg->var_table[i].var_type, "u16") == 0)
                    fprintf(cg->asm_file, "    movzx rax, word [rbp - %d]\n", cg->var_table[i].stack_offset);
                else if (strcmp(cg->var_table[i].var_type, "u8") == 0)
                    fprintf(cg->asm_file, "    movzx rax, byte [rbp - %d]\n", cg->var_table[i].stack_offset);

                return;
            }
        }
        fprintf(stderr, "Error: unknown variable '%s'\n", node->name);
        exit(1);
    }

    if (node->type == NODE_BINOP) {
        codegen_expr(cg, node->left);          // left -> rax
        fprintf(cg->asm_file, "    push rax\n"); // sauvegarde left
        codegen_expr(cg, node->right);         // right -> rax
        fprintf(cg->asm_file, "    push rax\n"); // sauvegarde right

        switch (node->op) {
            case '+': fprintf(cg->asm_file, "    call _add\n"); break;
            case '-': fprintf(cg->asm_file, "    call _sub\n"); break;
            case '*': fprintf(cg->asm_file, "    call _mul\n"); break;
            case '/':
                if (strncmp(node->var_type, "u", 1) == 0)
                    fprintf(cg->asm_file, "    call _udiv\n");
                else
                    fprintf(cg->asm_file, "    call _div\n");
                break;
                    }
        fprintf(cg->asm_file, "    pop rax\n");
    }
}