#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"
#include <stdio.h>

typedef struct {
    FILE *asm_file;
    int stack_offset;
} Codegen;

Codegen codegen_new(FILE *asm_file);
void codegen_var_decl(Codegen *cg, Node *node);
void codegen_prologue(Codegen *cg);
void codegen_epilogue(Codegen *cg);

#endif