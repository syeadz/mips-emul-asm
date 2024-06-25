# MIPS Emulator and Assembler

Very stripped down emulator and assembler for MIPS instruction set. The emulator uses a TUI interface to interact with the user. The assembler is very basic and only supports 5 commands.

Available commands:

* `add`
* `j`
* `beq`
* `lw`
* `sw`

## Building

To build the emulator, run `make` from root directory. The emulator requires the `ncurses` library to be installed.

To build the assembler, go to the `assembler/` folder and run `make`.

If you get any errors, check if you have the necessary libraries installed. You can install `ncurses` using `sudo apt-get install libncurses5-dev`. Also, make sure you have the `build/` directory created in the root directory. You can also use `make setup` to create the directory.

### Testing

There are unit tests in place for the emulator. To run them, run `make test`.

## Usage

### Emulator

Run the emulator with `./main` or use `make run`. NOTE: the TUI is a set size and will not resize if the terminal window is resized or too small. Make sure to have a large enough terminal window. If it is too small, press `q` to quit the emulator and rerun it with a larger terminal window. You should be able to see the available commands on the bottom upon startup.

You can then use the following commands (addresses are in hex):

* `n`: step through the program one instruction at a time
* `l`: load a program from a file. Will prompt for the file name and a memory address to load the program into.
* `j`: change PC to a specific memory address. Will prompt for the address.
* `m`: jump to a specific memory address. Will prompt for the address.
* `h`: print help
* `q`: quit the emulator

### Assembler

For the assembler, run `./asm <input file> <output file>` to produce a binary file. Note that the assembler is very basic and only supports 5 commands and nothing else (no comments, labels, or hex constants are currently supported). Check the `assembler/test.asm` or `assembler/add.asm` files for an example on how the code should look.

### Example

We will turn a simple assembly program into machine code using the assembler and then run it in the emulator. The program is a simple one that loads 2 numbers from memory, adds them together, stores the result in memory, and finally branches if 2 registers are equal. Note that the branch will move 9 words + 1 instruction ahead if the condition is met.

Contents of `assembler/add.asm`:

```asm
lw $t1, 48($zero)
lw $t2, 52($zero)
add $t0, $t1, $t2
sw $t0, 56($zero)
lw $t3, 56($zero)
beq $t0, $t3, 9
```

We will start by assembling the program:

```bash
$ make -C assembler/ # build the assembler, make sure build/ directory is created, otherwise run `make setup`
$ ./assembler/asm assembler/add.asm add.bin # assemble the program
```

You can inspect the binary file using `xxd -g4 -c4 add.bin` (you will need to install the `xxd` tool) if you want to see the machine code. You can take one of the lines and run it through the `num_to_instr` tool to see the machine code broken down. You will need to run `make -C tools/` to build the tools first. Example output is below:

```bash
$ xxd -g4 -c4 add.bin
00000000: 8c090030  ...0
00000004: 8c0a0034  ...4
00000008: 012a4020  .*@ 
0000000c: ac080038  ...8
00000010: 8c0b0038  ...8
00000014: 310b003c  1..<

$ ./tools/num_to_instr i 0x8c090030
100011 00000 01001 0000000000110000
Opcode: 35 (0x23), Rs: 0 (0x0), Rt: 9 (0x9), Imm: 48 (0x30)
```

We also require an extra memory file to load onto the emulator. We can create it by running:

```bash
perl -ne 'print pack("B32", $_)' < assembler/memory.txt > memory.bin
```

This will create a binary file with 1 and 2 for the first 2 memory addresses. You can also inspect this file using `xxd -g4 -c4 memory.bin`:

```bash
$ xxd -g4 -c4 memory.bin
00000000: 00000001  ....
00000004: 00000002  ....
```

Now we can run the emulator using `./main` or `make run`. Remember to have a large enough terminal window. You should be able to see available commands at the bottom on startup. Press `q` if not and resize the terminal and restart the emulator. We can then load the program and memory files. Run the following commands in the emulator (values are in hex):

```
l
add.bin
0

l
memory.bin
30
```

You can then step through the program using `n` and see the results in the memory window. The program should load the numbers 1 and 2, add them together, store the result in memory, and then branch if the 2 registers are equal. It should also display the instructions themselves on the right side of the terminal. Keep track of the register names and memory addresses to see the program in action. Note that the branch will move 9 words + 1 instruction ahead if the condition is met. This means it will branch to address `0x3c` by the end of the program.

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
