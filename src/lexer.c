#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"

Lexer lexer_new(const char *src) {
    Lexer l;
    l.src  = src;
    l.pos  = 0;
    l.line = 1;
    l.col  = 1;
    l.current_char   = src[0];
    return l;
}

char current(Lexer *l) {
    return l->current_char;
}

char peek(Lexer *l) {
    return l->src[l->pos + 1];
}

void advance(Lexer *l) {
    l->pos++;
    l->current_char = l->src[l->pos];

    if (l->current_char == '\n') {
        l->line += 1;
        l->col = 1;
    } else {
        l->col ++;
    }
}

void skip_whitespace(Lexer *l) {
    while (1) {
        switch (l->current_char) {
            case ' ':  advance(l); break;
            case '\t': advance(l); break;
            case '\n': advance(l); break;
            case '\r': advance(l); break;
            default: return;
        }
    }
}

void read_comment(Lexer *l) {
    advance(l); // saute le premier /

    switch (l->current_char) {
        case '/': {
            while (l->current_char != '\n' && l->current_char != '\0') advance(l);
            break;
        }

        case '*':
            advance(l);
            while (!(l->current_char == '*' && peek(l) == '/')) {
                if (l->current_char == '\0') break;
                advance(l);
            }
            advance(l);
            advance(l);
            break;
    }
}

Token read_string(Lexer *l) {
    Token t;
    t.type = TOKEN_STRING;
    advance(l); // "
    int i = 0;
    while (l->current_char != '"' && l->current_char != '\0') {
        t.value[i++] = l->current_char;
        advance(l);
    }
    t.value[i] = '\0';
    advance(l); // "
    return t;
}

Token read_char(Lexer *l) {
    Token t;
    t.type = TOKEN_CHAR;
    advance(l); // '
    t.value[0] = l->current_char;
    t.value[1] = '\0';
    advance(l); // 1 seul char
    advance(l); // '
    return t;
}

Token read_indent(Lexer *l) {
    Token t;
    int i = 0;

    while (isalpha(l->current_char) || isdigit(l->current_char) || l->current_char == '_') {
        t.value[i++] = l->current_char;
        advance(l);
    }
    t.value[i] = '\0';

    if (strcmp(t.value, "i8")  == 0 || strcmp(t.value, "i16") == 0 ||
        strcmp(t.value, "i32") == 0 || strcmp(t.value, "i64") == 0 ||
        strcmp(t.value, "u8")  == 0 || strcmp(t.value, "u16") == 0 ||
        strcmp(t.value, "u32") == 0 || strcmp(t.value, "u64") == 0 ||
        strcmp(t.value, "f32") == 0 || strcmp(t.value, "f64") == 0 ||
        strcmp(t.value, "const") == 0) {
        t.type = TOKEN_TYPE;
    } else {
        t.type = TOKEN_IDENT;
    }

    return t;
}

Token next_token(Lexer *l) {
    skip_whitespace(l);

    switch (l->current_char) {
        case '\0': {
            Token t;
            t.type = TOKEN_EOF;
            t.value[0] = '\0';

            return t;
        }

        case '=': {
            Token t;
            t.type = TOKEN_EQUAL;
            t.value[0] = '=';
            t.value[1] = '\0';
            advance(l);

            return t;
        }

        case ';': {
            Token t;
            t.type = TOKEN_SEMI;
            t.value[0] = ';';
            t.value[1] = '\0';
            advance(l);

            return t;
        }

        case '/':
            read_comment(l);
            return next_token(l);

        case '"':  return read_string(l);

        case '\'': return read_char(l);

        default:
            if (isalpha(l->current_char) || l->current_char == '_') return read_indent(l);
    }

    // caractère inconnu
    Token t;
    t.type = TOKEN_EOF;
    t.value[0] = '\0';
    return t;
}