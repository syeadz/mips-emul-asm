CC = gcc
# Using -g for debugging and -Wall -Wextra for warnings
CFLAGS = -g -Wall -Wextra

ODIR = build

# no libraries required currently
LIBS = 

.PHONY: all build build_test test clean setup

all: build build_test

# builds main program
build: $(ODIR)/mips_emul.o $(ODIR)/main.o main

# builds test for mips_emul
build_test: $(ODIR)/mips_emul.o $(ODIR)/mips_emul_test.o $(ODIR)/emultest

# Builds object files
$(ODIR)/main.o: main.c mips_emul.h mips_emul.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/mips_emul.o: mips_emul.c mips_emul.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(ODIR)/mips_emul.o $(ODIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/mips_emul_test.o: mips_emul_test.c mips_emul.h minunit.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/emultest: $(ODIR)/mips_emul.o $(ODIR)/mips_emul_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# create build directory
setup:
	mkdir -p $(ODIR)

# runs test file
test: $(ODIR)/emultest
	./$(ODIR)/emultest

# removes object files and test file
clean:
	rm -f $(ODIR)/*.o $(ODIR)/emultest
	rm main