CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/lexer.c src/ast.c src/parser.c src/codegen.c src/file.c
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
BIN = build/vaultc

all: build $(BIN)

build:
	mkdir -p build
	mkdir -p asm

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	./$(BIN)

run_asm:
	nasm -f elf64 asm/output.asm -o asm/output.o
	ld asm/output.o -o asm/output
	./asm/output

full: clean all run

clean:
	rm -rf build

.PHONY: all clean run full