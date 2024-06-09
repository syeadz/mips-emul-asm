CC = gcc
# Using -g for debugging and -Wall -Wextra for warnings
CFLAGS = -g -Wall -Wextra

# no libraries required currently
LIBS = 

.PHONY: all instr num clean

all: instr num

instr: instr_to_num.c
	$(CC) $(CFLAGS) -o instr_to_num instr_to_num.c $(LIBS)

num: num_to_instr.c
	$(CC) $(CFLAGS) -o num_to_instr num_to_instr.c $(LIBS)

# removes object files and test file
clean:
	rm instr_to_num num_to_instr