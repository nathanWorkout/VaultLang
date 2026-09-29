CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/lexer.c src/ast.c src/parser.c src/file.c \
      src/codegen/codegen.c \
      src/codegen/codegen_var.c \
      src/codegen/codegen_print.c \
      src/codegen/codegen_prologue.c

OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
BIN = build/vaultc

all: build $(BIN)

build:
	mkdir -p build

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run:
	./$(BIN)

run_asm:
	rm -f asm/output.o asm/output
	nasm -f elf64 asm/output.asm -o asm/output.o
	ld asm/output.o -o asm/output
	./asm/output

full: clean all run

clean:
	rm -rf build

.PHONY: all clean run full
