#include "../include/file.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "error: unable to open %s\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *src = malloc(size + 1);
    fread(src, 1, size, f);
    src[size] = '\0';
    fclose(f);

    return src;
}