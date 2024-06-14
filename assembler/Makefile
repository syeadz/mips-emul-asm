CC = gcc
CFLAGS = -g -Wall -Wextra

ODIR = ../build

ASSEMBLER_OBJS = $(ODIR)/mips_asm.o $(ODIR)/parser.o $(ODIR)/tokenizer.o $(ODIR)/utils.o

ASSEMBLER_EXEC = asm

.PHONY: all build clean setup

all: build

build: $(ASSEMBLER_EXEC)

$(ASSEMBLER_EXEC): $(ASSEMBLER_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/utils.o: ../utils.c ../utils.h
	$(CC) -c -o $@ $< $(CFLAGS)

setup:
	mkdir -p build

clean:
	rm -f $(ASSEMBLER_OBJS) $(ASSEMBLER_EXEC)
