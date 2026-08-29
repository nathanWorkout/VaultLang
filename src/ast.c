#include <string.h>
#include "../include/ast.h"

Node make_var_decl(char *var_type, char *name, char *value) {
    Node n;
    n.type = NODE_VAR_DECL;
    strncpy(n.var_type, var_type, 16);
    strncpy(n.name, name, 256);
    strncpy(n.value, value, 256);
    return n;
}