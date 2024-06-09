# MIPS Emulator and Assembler

Very stripped down emulator and assembler for MIPS instruction set.

Currently only supports 5 commands:

* add
* j
* beq
* lw
* sw

## Building

To build the emulator, run `make` from root directory. There are no external dependencies.

To build the assembler, go to the assembler folder and run `make`.

### Testing

There are unit tests in place for the emulator. To run them, run `make test`.

## Usage

### Emulator

From the main.c function, specify the binary files to read into memory.

### Assembler

For the assembler, run `./asm <input file> <output file>` to produce a binary file. Note that the assembler is very basic and only supports 5 commands and nothing else.

## Useful Links

* [MIPS Reference Data](https://courses.cs.washington.edu/courses/cse378/09au/MIPS_Green_Sheet.pdf)
* [MIPS Reference Sheet](https://uweb.engr.arizona.edu/~ece369/Resources/spim/MIPSReference.pdf)
* [Writing an Assembler](https://student.cs.uwaterloo.ca/~cs241/slides/sylvie/Sylvie-L5.pdf)
