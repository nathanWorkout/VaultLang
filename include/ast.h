#ifndef AST_H
#define AST_H
#include "../include/lexer.h"

typedef enum {
    NODE_VAR_DECL,
    NODE_PRINT,
    NODE_BINOP,
    NODE_INT_LIT,
    NODE_IDENT,
    NODE_STRING_LIT,
} NodeType;

typedef struct Node {
    // variables
    NodeType  type;
    char      var_type[16];
    char      name[256];
    char      value[256];
    TokenType value_type;
    int       str_id;
    // binop
    struct Node *left;
    struct Node *right;
    char         op;
} Node;

Node *make_var_decl(char *var_type, char *name, Node *expr, TokenType value_type);
Node *make_print(char *value, TokenType value_type);
Node *make_binop(char op, Node *left, Node *right, char *var_type);
Node *make_int_lit(char *value);
Node *make_ident(char *name);
Node *make_string_lit(char *value);
void  node_free(Node *n);

#endif