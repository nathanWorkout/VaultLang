#include <string.h>
#include "../../include/codegen.h"

Codegen codegen_new(FILE *asm_file) {
    Codegen cg;
    cg.str_table_count = 0;
    cg.asm_file = asm_file;
    cg.stack_offset = 0;       // sert juste pour les variables pour l'instant
    cg.data_section[0] = '\0'; // buffer pour les strings en memoire avant de le write
    cg.data_offset = 0;        // offset des strings
    cg.str_count = 0;          // numero de la prochaine string
    cg.var_table_count = 0;
    return cg;
}

void codegen_node(Codegen *cg, Node *node) {
    switch (node->type) {
        case NODE_VAR_DECL: codegen_var_decl(cg, node); break;
        case NODE_PRINT:    codegen_print(cg, node);    break;
    }
}