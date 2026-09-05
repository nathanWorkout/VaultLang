#include <string.h>
#include "../include/ast.h"

Node make_var_decl(char *var_type, char *name, char *value, TokenType value_type) {
    Node n;
    n.type = NODE_VAR_DECL;
    strncpy(n.var_type, var_type, 16);
    strncpy(n.name, name, 256);
    strncpy(n.value, value, 256);
    n.value_type = value_type;
    return n;
}


Node make_print(char *value, TokenType value_type) {
    Node n;
    n.type = NODE_PRINT;
    strncpy(n.value, value, 256);
    n.value_type = value_type;
    return n;
}
