#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/ast.h"

static Node *parse_term(Parser *p);
static Node *parse_factor(Parser *p);
Node *parse_expr(Parser *p);

void parser_advance(Parser *p) {
    p->current = next_token(&p->lexer);
}

static Node *parse_term(Parser *p) {
    if (p->current.type == TOKEN_INT) {
        Node *n = make_int_lit(p->current.value);
        parser_advance(p);
        return n;
    }

    // nombre négatif 
    if (p->current.type == TOKEN_MINUS) {
        parser_advance(p); // -
        char val[257];
        snprintf(val, sizeof(val), "-%s", p->current.value); // met le - devant le nombre
        Node *n = make_int_lit(val); // creer un noeud avec le nombre negatif
        parser_advance(p); // passe le nombre
        return n;
    }

    if (p->current.type == TOKEN_IDENT) {
        Node *n = make_ident(p->current.value);
        parser_advance(p);
        return n;
    }

    // expression entre parenthèses
    if (p->current.type == TOKEN_LPAREN) {
        parser_advance(p); // (
        Node *n = parse_expr(p);
        if (p->current.type != TOKEN_RPAREN) {
            fprintf(stderr, "Error: expected ')'\n");
            exit(1);
        }
        parser_advance(p); // )
        return n;
    }

    if (p->current.type == TOKEN_STRING) {
        Node *n = make_string_lit(p->current.value);
        parser_advance(p);
        return n;
    }

    fprintf(stderr, "Error line %zu : unexpected token '%s'\n", p->lexer.line, p->current.value);
    exit(1);
}

// parse * et /
static Node *parse_factor(Parser *p) {
    Node *left = parse_term(p);

    while (p->current.type == TOKEN_STAR || p->current.type == TOKEN_SLASH) {
        char op = p->current.value[0];
        parser_advance(p);
        Node *right = parse_term(p);
        left = make_binop(op, left, right, ""); // "" car on sait pas encore si cest int ou unsigned
    }

    return left;
}

// parse + et - 
Node *parse_expr(Parser *p) {
    Node *left = parse_factor(p);

    while (p->current.type == TOKEN_PLUS || p->current.type == TOKEN_MINUS) {
        char op = p->current.value[0];
        parser_advance(p);
        Node *right = parse_factor(p);
        left = make_binop(op, left, right, "");
    }

    return left;
}

Node *parse_var_decl(Parser *p) {
    char var_type[16];
    char name[256];

    // type
    strncpy(var_type, p->current.value, 15); var_type[15] = '\0';
    parser_advance(p);

    // name
    if (p->current.type != TOKEN_IDENT) {
        fprintf(stderr, "Error line %zu : expected identifier\n", p->lexer.line);
        exit(1);
    }
    strncpy(name, p->current.value, 255); name[255] = '\0';
    parser_advance(p);

    // =
    if (p->current.type != TOKEN_EQUAL) {
        fprintf(stderr, "Error line %zu : expected '='\n", p->lexer.line);
        exit(1);
    }
    parser_advance(p);

    Node *expr = parse_expr(p);
    if (expr->type == NODE_BINOP) strncpy(expr->var_type, var_type, 15);

    // ;
    if (p->current.type != TOKEN_SEMI) {
        fprintf(stderr, "Error line %zu : expected ';'\n", p->lexer.line);
        exit(1);
    }
    parser_advance(p);

    return make_var_decl(var_type, name, expr, expr->value_type);
}

Node *parse_print(Parser *p) {
    parser_advance(p); // print

    if (p->current.type != TOKEN_LPAREN) {
        fprintf(stderr, "Error line %zu : expected '('\n", p->lexer.line);
        exit(1);
    }
    parser_advance(p); // (

    char value[256];
    TokenType value_type = p->current.type;
    strncpy(value, p->current.value, 255); value[255] = '\0';
    parser_advance(p); // value

    if (p->current.type != TOKEN_RPAREN) {
        fprintf(stderr, "Error line %zu : expected ')'\n", p->lexer.line);
        exit(1);
    }
    parser_advance(p); // )

    if (p->current.type != TOKEN_SEMI) {
        fprintf(stderr, "Error line %zu : expected ';'\n", p->lexer.line);
        exit(1);
    }
    parser_advance(p); // ;

    return make_print(value, value_type);
}

Node *parse(Parser *p) {
    if (p->current.type == TOKEN_TYPE) {
        if (strcmp(p->current.value, "print") == 0) {
            return parse_print(p);
        } else {
            return parse_var_decl(p);
        }
    }

    parser_advance(p);
    return NULL;
}