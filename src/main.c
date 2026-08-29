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

    FILE *out = fopen("asm/output.asm", "w");
    Codegen cg = codegen_new(out);

    codegen_prologue(&cg);

    while (p.current.type != TOKEN_EOF) {
        Node n = parse(&p);
        codegen_var_decl(&cg, &n);
    }

    codegen_epilogue(&cg);

    fclose(out);
    free(src);
    return 0;
}