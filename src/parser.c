#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/ast.h"

void parser_advance(Parser *p) {
    p->current = next_token(&p->lexer);
}

Node parse_var_decl(Parser *p) {
    char var_type[16];
    char name[256];
    char value[256];

    // type
    strncpy(var_type, p->current.value, 16);
    parser_advance(p);

    // name
    strncpy(name, p->current.value, 256);
    parser_advance(p);

    // =
    parser_advance(p);

    // value
    strncpy(value, p->current.value, 256);
    parser_advance(p);

    // ;
    if (p->current.type != TOKEN_SEMI) {
        fprintf(stderr, "Error line %zu : expected ';'\n", p->lexer.line);
        exit(1);
    }
    parser_advance(p);

    return make_var_decl(var_type, name, value);
}

Node parse(Parser *p) {
    if (p->current.type == TOKEN_TYPE) {
        return parse_var_decl(p);
    }

    Node n;
    n.type = -1;
    parser_advance(p);
    
    return n;
}
