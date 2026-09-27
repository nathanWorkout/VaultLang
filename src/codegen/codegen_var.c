#include <stdio.h>
#include <string.h>
#include "../../include/codegen.h"

// convertit les \n en syntaxe nasm valide
void codegen_collect_string(Codegen *cg, Node *node) {
    node->str_id = cg->str_count++;
    char nasm_str[512];
    int last_was_newline = 0;
    int i = 0;
    nasm_str[i++] = '"';

    for (int j = 0; node->value[j] != '\0'; j++) {
        // converssion \n en 10 ASCII
        if (node->value[j] == '\n') {
            nasm_str[i++] = '"';
            i += sprintf(nasm_str + i, ", 10");
            // si il y a encore des chars :
            if (node->value[j+1] != '\0') {
                i += sprintf(nasm_str + i, ", \"");
            }
            // flag pour savoir si la string se termine par \n 
            last_was_newline = (node->value[j+1] == '\0');
        } else {
            nasm_str[i++] = node->value[j]; // si c'est un caractere normal, copie dans cette variable
            last_was_newline = 0;
        }
    }
    if (!last_was_newline) nasm_str[i++] = '"';
    nasm_str[i] = '\0'; // on termine le buffer car la string est finie

    cg->data_offset += sprintf(
        cg->data_section + cg->data_offset,
        "    str%d db %s, 0\n",
        node->str_id, nasm_str
    );

    // on enregistre la string dans la str_table (3 param)
    strncpy(cg->str_table[cg->str_table_count].name, node->name, 256); // nom
    strncpy(cg->str_table[cg->str_table_count].value, node->value, 256); // valeure
    cg->str_table[cg->str_table_count].str_id = node->str_id; // id (str0, str1...)
    cg->str_table_count++;
}

static void register_var(Codegen *cg, Node *node) {
    strncpy(cg->var_table[cg->var_table_count].name, node->name, 256);
    strncpy(cg->var_table[cg->var_table_count].var_type, node->var_type, 16);
    cg->var_table[cg->var_table_count].stack_offset = cg->stack_offset;
    cg->var_table_count++;
}

// note : faire tout les types de 0 a 256
void codegen_var_decl(Codegen *cg, Node *node) {
    if (strcmp(node->var_type, "i8") == 0) {
        cg->stack_offset += 1;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov byte %s, %s\n", node->name, node->value);

        register_var(cg, node);
    }
    else if (strcmp(node->var_type, "i16") == 0) {
        cg->stack_offset = (cg->stack_offset + 1) & ~1;
        cg->stack_offset += 2;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov word %s, %s\n", node->name, node->value);

        register_var(cg, node);
    }
    else if (strcmp(node->var_type, "i32") == 0) {
        cg->stack_offset = (cg->stack_offset + 3) & ~3;
        cg->stack_offset += 4;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov dword %s, %s\n", node->name, node->value);

        register_var(cg, node);
    }
    else if (strcmp(node->var_type, "i64") == 0) {
        cg->stack_offset = (cg->stack_offset + 7) & ~7;
        cg->stack_offset += 8;
        fprintf(cg->asm_file, "%%define %s [rbp - %d]\n", node->name, cg->stack_offset);
        fprintf(cg->asm_file, "    mov qword %s, %s\n", node->name, node->value);

        register_var(cg, node);
    }
    else if (strcmp(node->var_type, "str") == 0) {
        codegen_collect_string(cg, node);
        strncpy(cg->str_table[cg->str_table_count].name, node->name, 256);
        strncpy(cg->str_table[cg->str_table_count].value, node->value, 256);
        cg->str_table[cg->str_table_count].str_id = node->str_id;
        cg->str_table_count++;
    }
}