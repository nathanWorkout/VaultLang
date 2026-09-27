#include <stdio.h>
#include <string.h>
#include "../../include/codegen.h"

void codegen_print(Codegen *cg, Node *node) {
    // on cherche d'abord dans les nombres
    for (int i = 0; i < cg->var_table_count; i++) {
        if (strcmp(cg->var_table[i].name, node->value) == 0) {
            if (strcmp(cg->var_table[i].var_type, "i64") == 0) {
                fprintf(cg->asm_file, "    mov rax, [rbp - %d]\n", cg->var_table[i].stack_offset);
            }
            // movsxd = move with sign extension il etend correctement les types plu spetit vers rax en preservant le signe
            else if (strcmp(cg->var_table[i].var_type, "i32") == 0) {
                fprintf(cg->asm_file, "    movsxd rax, dword [rbp - %d]\n", cg->var_table[i].stack_offset);
            }
            else if (strcmp(cg->var_table[i].var_type, "i16") == 0) {
                fprintf(cg->asm_file, "    movsx rax, word [rbp - %d]\n", cg->var_table[i].stack_offset);
            }
            else if (strcmp(cg->var_table[i].var_type, "i8") == 0) {
                fprintf(cg->asm_file, "    movsx rax, byte [rbp - %d]\n", cg->var_table[i].stack_offset);
            }
            fprintf(cg->asm_file, "    call itoa\n");
            return;
        }
    }

    // si c'est pas un int on cherche dans les strings
    int str_id = -1;
    int len = 0;
    if (node->value_type == TOKEN_STRING || node->value_type == TOKEN_IDENT) {
        for (int i = 0; i < cg->str_table_count; i++) {
            char *key = node->value_type == TOKEN_STRING ? cg->str_table[i].value : cg->str_table[i].name;
            if (strcmp(key, node->value) == 0) {
                str_id = cg->str_table[i].str_id;
                len = strlen(cg->str_table[i].value);
                break;
            }
        }
    }

    if (str_id == -1) {
        fprintf(stderr, "Error: unknown variable in print\n");
        return;
    }

    fprintf(cg->asm_file, "    mov rax, 1\n");
    fprintf(cg->asm_file, "    mov rdi, 1\n");
    fprintf(cg->asm_file, "    mov rsi, str%d\n", str_id);
    fprintf(cg->asm_file, "    mov rdx, %d\n", len);
    fprintf(cg->asm_file, "    syscall\n");
}