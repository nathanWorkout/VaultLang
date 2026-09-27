#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"
#include <stdio.h>

typedef struct {
    char name[256];
    char value[256];
    int  str_id;
} StrEntry;

typedef struct {
    char name[256];
    char var_type[16];
    int  stack_offset;
} VarEntry;

typedef struct {
    FILE     *asm_file;
    int       stack_offset;
    char      data_section[4096]; // buffer
    int       data_offset;        // offset buffer
    int       str_count;
    StrEntry  str_table[256];
    int       str_table_count;
    VarEntry  var_table[256];     
    int       var_table_count; 
} Codegen;

Codegen codegen_new(FILE *asm_file);
void codegen_var_decl(Codegen *cg, Node *node);
void codegen_collect_string(Codegen *cg, Node *node);
void codegen_print(Codegen *cg, Node *node);
void codegen_prologue(Codegen *cg);
void codegen_epilogue(Codegen *cg);
void codegen_node(Codegen *cg, Node *node);

#endif