#ifndef LEXER_H
#define LEXER_H
#include <stdlib.h>

typedef enum {
    TOKEN_TYPE,
    TOKEN_IDENT,
    TOKEN_EQUAL,
    TOKEN_INT, 
    TOKEN_SEMI,
    TOKEN_STRING, // "a"
    TOKEN_CHAR,   // 'a'
    TOKEN_EOF,
} TokenType;

typedef struct Token {
    TokenType type;
    char value[256];
} Token;

typedef struct {
    const char *src;
    size_t pos;
    size_t line;
    size_t col;
    char current_char;
} Lexer;

Lexer lexer_new(const char *src);
char current(Lexer *l);
char peek(Lexer *l);
void advance(Lexer *l);
void skip_whitespace(Lexer *l);
void read_comment(Lexer *l);
Token read_string(Lexer *l);
Token read_char(Lexer *l);
Token read_indent(Lexer *l);
Token next_token(Lexer *l);

#endif