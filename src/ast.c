#include <stdlib.h>
#include <string.h>
#include "../include/ast.h"

static Node *node_new(NodeType type) {
    Node *n = malloc(sizeof(Node)); // pour que ca survive en dehors du scope de la fonction (au lieu de la stack c'est stocker sur le heap)
    n->type  = type;
    n->left  = NULL;
    n->right = NULL;
    n->op    = 0;
    n->str_id = 0;
    n->var_type[0] = '\0';
    n->name[0]     = '\0';
    n->value[0]    = '\0';
    return n;
}

Node *make_var_decl(char *var_type, char *name, Node *expr, TokenType value_type) {
    Node *n = node_new(NODE_VAR_DECL);
    strncpy(n->var_type, var_type, 15); n->var_type[15] = '\0';
    strncpy(n->name, name, 255);        n->name[255]    = '\0';
    strncpy(n->value, expr->value, 255); n->value[255]  = '\0';
    n->left       = expr;   // l'expression est dans left
    n->value_type = value_type;
    return n;
}

Node *make_print(char *value, TokenType value_type) {
    Node *n = node_new(NODE_PRINT);
    strncpy(n->value, value, 255); n->value[255] = '\0';
    n->value_type = value_type;
    return n;
}

Node *make_binop(char op, Node *left, Node *right, char *var_type) {
    Node *n = node_new(NODE_BINOP);
    n->op    = op;
    n->left  = left;
    n->right = right;
    strncpy(n->var_type, var_type, 15); n->var_type[15] = '\0';
    return n;
}

Node *make_int_lit(char *value) {
    Node *n = node_new(NODE_INT_LIT);
    strncpy(n->value, value, 255); n->value[255] = '\0';
    return n;
}

Node *make_ident(char *name) {
    Node *n = node_new(NODE_IDENT);
    strncpy(n->name, name, 255); n->name[255] = '\0';
    return n;
}

Node *make_string_lit(char *value) {
    Node *n = node_new(NODE_STRING_LIT);
    strncpy(n->value, value, 255); n->value[255] = '\0';
    n->value_type = TOKEN_STRING;
    return n;
}

void node_free(Node *n) {
    if (!n) return;
    node_free(n->left);
    node_free(n->right);
    free(n);
}