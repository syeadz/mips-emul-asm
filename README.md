# MIPS Emulator and Assembler

Very stripped down emulator and assembler for MIPS instruction set.

Currently only supports 5 commands:

* `add`
* `j`
* `beq`
* `lw`
* `sw`

## Building

To build the emulator, run `make` from root directory. There are no external dependencies.

To build the assembler, go to the `assembler/` folder and run `make`.

### Testing

There are unit tests in place for the emulator. To run them, run `make test`.

## Usage

### Emulator

From the main.c function, specify the binary files to read into memory.

### Assembler

For the assembler, run `./asm <input file> <output file>` to produce a binary file. Note that the assembler is very basic and only supports 5 commands and nothing else (no comments or labels currently supported). Check the `assembler/test.asm` file for an example on how it should look.

## Tools

There are 2 command line utilities found in the `tools/` folder. Run `make` in the folder to create the binaries.

### instr_to_num

Converts the arguments for an instruction (opcode, rs, rd, etc) and prints its respective machine code equivalent. Arguments can be given as hex with prefix `0x` or as binary with prefix `0b`.

Usage:

* for r type instruction: `./instr_to_num r <opcode> <rs> <rt> <rd> <shamt> <funct>`
* for i type instruction: `./instr_to_num i <opcode> <rs> <rt> <immediate>`
* for j type instruction: `./instr_to_num j <opcode> <address>`

### num_to_instr

Converts the machine code instruction (can be given as hex) into its separate sections (opcode, rs, rt, etc).

Usage: `./num_to_insr <instruction type, (r or i or j)> <number>`

## Useful Links

* [MIPS Reference Data](https://courses.cs.washington.edu/courses/cse378/09au/MIPS_Green_Sheet.pdf)
* [MIPS Reference Sheet](https://uweb.engr.arizona.edu/~ece369/Resources/spim/MIPSReference.pdf)
* [MIPS Partial Instruction Set](https://ecs-network.serv.pacific.edu/ecpe-170/tutorials/mips-instruction-set)
* [Writing an Assembler](https://student.cs.uwaterloo.ca/~cs241/slides/sylvie/Sylvie-L5.pdf)

## Acknowledgements

* Special thanks to [Emulator 101](http://www.emulator101.com/) as the structure of the emulator is taken from there
* Special thanks to David Siñuela Pastor for the [minunit](https://github.com/siu/minunit) single header C unit testing framework
