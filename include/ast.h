#ifndef AST_H
#define AST_H
#include "../include/lexer.h"

typedef enum {
    NODE_VAR_DECL,
    NODE_PRINT,
} NodeType;

typedef struct {
    NodeType  type;
    char      var_type[16];
    char      name[256];
    char      value[256];
    TokenType value_type;
    int       str_id;
} Node;

Node make_var_decl(char *var_type, char *name, char *value, TokenType value_type);
Node make_print(char *value, TokenType value_type);
#endif
