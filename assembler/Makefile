CC = gcc
CFLAGS = -g -Wall -Wextra

ODIR = ../build

ASSEMBLER_OBJS = $(ODIR)/mips_asm.o $(ODIR)/parser.o $(ODIR)/tokenizer.o $(ODIR)/utils.o 

ASSEMBLER_EXEC = asm
ASSEMBLER_TEST = asmtest

.PHONY: all build clean setup test

all: setup build

build: $(ASSEMBLER_EXEC)

test: $(ODIR)/$(ASSEMBLER_TEST)
	./$(ODIR)/$(ASSEMBLER_TEST)

$(ASSEMBLER_EXEC): $(ASSEMBLER_OBJS) $(ODIR)/main_asm.o
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/$(ASSEMBLER_TEST): $(ASSEMBLER_OBJS) $(ODIR)/mips_asm_test.o
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/utils.o: ../utils.c ../utils.h
	$(CC) -c -o $@ $< $(CFLAGS)

setup:
	mkdir -p $(ODIR)

clean:
	rm -f $(ASSEMBLER_OBJS) $(ASSEMBLER_EXEC) $(ODIR)/$(ASSEMBLER_TEST) $(ODIR)/main_asm.o  $(ODIR)/mips_asm_test.o