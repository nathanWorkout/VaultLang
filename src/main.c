#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"

int main() {
    const char *src = "i8 test = \"abc\";";
    Lexer l = lexer_new(src);

    Token t;

    while ((t = next_token(&l)).type != TOKEN_EOF) {
        printf("type: %d  value: %s\n", t.type, t.value);
    }

    return 0;
}