CC = gcc
# Using -g for debugging and -Wall -Wextra for warnings
CFLAGS = -g -Wall -Wextra

ODIR = build

# no libraries required currently
LIBS = 

# Currently implements test build only

all: $(ODIR)/mips_emul.o $(ODIR)/mips_emul_test.o $(ODIR)/emultest


# builds test files
$(ODIR)/mips_emul.o: mips_emul.c mips_emul.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/mips_emul_test.o: mips_emul_test.c mips_emul.h minunit.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/emultest: $(ODIR)/mips_emul.o $(ODIR)/mips_emul_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: test clean setup

# create build directory
setup:
	mkdir $(ODIR)

# runs test file
test: $(ODIR)/emultest
	./$(ODIR)/emultest


clean:
	rm -f $(ODIR)/*.o $(ODIR)/emultest