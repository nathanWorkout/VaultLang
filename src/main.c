#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"
#include "../include/ast.h"
#include "../include/codegen.h"
#include "../include/parser.h"
#include "../include/file.h"

int main() {
    char *src = read_file("tests/hello.vl");

    Lexer l = lexer_new(src);

    Parser p;
    p.lexer = l;
    p.current = next_token(&p.lexer);

    Node nodes[256];
    int node_count = 0;

    while (p.current.type != TOKEN_EOF) {
        nodes[node_count++] = parse(&p);
    }

    FILE *out = fopen("asm/output.asm", "w");
    Codegen cg = codegen_new(out);

    // collecte
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].type == NODE_VAR_DECL && strcmp(nodes[i].var_type, "str") == 0) {
            codegen_var_decl(&cg, &nodes[i]);
        } else if (nodes[i].type == NODE_PRINT && nodes[i].value_type == TOKEN_STRING) {
            codegen_collect_string(&cg, &nodes[i]);
        }
    }

    codegen_prologue(&cg);

    // generation
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].type == NODE_VAR_DECL && strcmp(nodes[i].var_type, "str") == 0) continue; // strings deja traiter lors de la collecte
        codegen_node(&cg, &nodes[i]);
    }

    codegen_epilogue(&cg);

    fclose(out);
    free(src);
    return 0;
}