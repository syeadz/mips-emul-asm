CC = gcc
# Using -g for debugging and -Wall -Wextra for warnings
CFLAGS = -g -Wall -Wextra

ODIR = build

# no libraries required currently
LIBS =

# check if OS is Windows_NT to use pdcurses instead of ncurses
ifeq ($(OS),Windows_NT)
    TUI_LIBS = -lpdcurses
else
    TUI_LIBS = -lncurses
endif

.PHONY: all build build_test test clean setup run

all: build build_test

# builds main program
build: $(ODIR)/mips_emul.o $(ODIR)/tui.o $(ODIR)/main.o main

# builds test for mips_emul
build_test: $(ODIR)/mips_emul.o $(ODIR)/mips_emul_test.o $(ODIR)/emultest

# Builds object files
$(ODIR)/main.o: main.c mips_emul.h mips_emul.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/mips_emul.o: mips_emul.c mips_emul.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/tui.o: tui.c tui.h utils.h utils.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/utils.o: utils.c utils.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(ODIR)/mips_emul.o $(ODIR)/tui.o $(ODIR)/utils.o $(ODIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(TUI_LIBS)

$(ODIR)/mips_emul_test.o: mips_emul_test.c mips_emul.h minunit.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/emultest: $(ODIR)/mips_emul.o $(ODIR)/mips_emul_test.o $(ODIR)/utils.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# create build directory
setup:
	mkdir -p $(ODIR)

run: main
	./main

# runs test file
test: $(ODIR)/emultest
	./$(ODIR)/emultest

# removes object files and test file
clean:
	rm -f $(ODIR)/*.o $(ODIR)/emultest
	rm main