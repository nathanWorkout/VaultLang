#ifndef AST_H
#define AST_H

typedef enum {
    NODE_VAR_DECL,
} NodeType;

typedef struct {
    NodeType type;
    char var_type[16];
    char name[256];
    char value[256];
} Node;

Node make_var_decl(char *var_type, char *name, char *value);

#endif