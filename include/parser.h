#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_advance(Parser *p);
Node parse_var_decl(Parser *p);
Node parse(Parser *p);

#endif
