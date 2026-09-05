#include <string.h>
#include "../include/parser.h"
#include "../include/codegen.h"

Codegen codegen_new(FILE *asm_file) {
    Codegen cg;
    cg.str_table_count = 0;
    cg.asm_file = asm_file;
    cg.stack_offset = 0;
    cg.data_section[0] = '\0';
    cg.data_offset = 0;
    cg.str_count = 0;
    return cg;
}

void codegen_prologue(Codegen *cg) {
    fprintf(cg->asm_file, "section .data\n");
    fprintf(cg->asm_file, "%s\n", cg->data_section);
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

void codegen_collect_string(Codegen *cg, Node *node) {
    node->str_id = cg->str_count++;
    // au lieu de stocker la string brute on la convertit pour nasm
    char nasm_str[512];
    int i = 0;
    int last_was_newline = 0;

    nasm_str[i++] = '"'; 
    for (int j = 0; node->value[j] != '\0'; j++) {
        if (node->value[j] == '\n') {
            nasm_str[i++] = '"'; 
            i += sprintf(nasm_str + i, ", 10");
            if (node->value[j+1] != '\0') {
                i += sprintf(nasm_str + i, ", \""); // rouvre seulement si ya encore des chars
            }
            last_was_newline = (node->value[j+1] == '\0');
        } else {
            nasm_str[i++] = node->value[j];
            last_was_newline = 0;
        }
    }
    if (!last_was_newline) nasm_str[i++] = '"'; // ferme seulement si pas deja ferme
    nasm_str[i] = '\0';

    cg->data_offset += sprintf(
        cg->data_section + cg->data_offset,
        "    str%d db %s, 0\n",
        node->str_id, nasm_str
    );

    // On met tout dans la table
    strncpy(cg->str_table[cg->str_table_count].name, node->name, 256);
    strncpy(cg->str_table[cg->str_table_count].value, node->value, 256);
    cg->str_table[cg->str_table_count].str_id = node->str_id;
    cg->str_table_count++;
}

void codegen_var_decl(Codegen *cg, Node *node) {
    if (strcmp(node->var_type, "i8") == 0) {
        cg->stack_offset += 1;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov byte %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "i16") == 0) {
        // alignement sinon ca donne un offset faut dans la stack
        cg->stack_offset = (cg->stack_offset + 1) & ~1;
        cg->stack_offset += 2;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov word %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "i32") == 0) {
        cg->stack_offset = (cg->stack_offset + 3) & ~3;
        cg->stack_offset += 4;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov dword %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "i64") == 0) {
        cg->stack_offset = (cg->stack_offset + 7) & ~7;
        cg->stack_offset += 8;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov qword %s, %s\n", node->name, node->value);
    }
    else if (strcmp(node->var_type, "str") == 0) {
        codegen_collect_string(cg, node);
        strncpy(cg->str_table[cg->str_table_count].name, node->name, 256);
        strncpy(cg->str_table[cg->str_table_count].value, node->value, 256);
        cg->str_table[cg->str_table_count].str_id = node->str_id;
        cg->str_table_count++;
    }
}

void codegen_print(Codegen *cg, Node *node) {
    int str_id = -1; // car ca commence a str0
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
        fprintf(stderr, "Error: unknown string in print\n");
        return;
    }

    fprintf(cg->asm_file, "    mov rax, 1\n");
    fprintf(cg->asm_file, "    mov rdi, 1\n");
    fprintf(cg->asm_file, "    mov rsi, str%d\n", str_id);
    fprintf(cg->asm_file, "    mov rdx, %d\n", len);
    fprintf(cg->asm_file, "    syscall\n");
}

void codegen_node(Codegen *cg, Node *node) {
    switch (node->type) {
        case NODE_VAR_DECL: codegen_var_decl(cg, node); break;
        case NODE_PRINT:    codegen_print(cg, node);    break;
    }
}
