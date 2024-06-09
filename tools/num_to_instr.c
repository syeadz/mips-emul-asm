#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

long parse_number(const char *arg)
{
    char *endptr;
    long number = 0;

    if (strlen(arg) > 2 && arg[0] == '0')
    {
        if (arg[1] == 'x' || arg[1] == 'X')
        {
            // Hexadecimal
            number = strtol(arg, &endptr, 16);
        }
        else if (arg[1] == 'b' || arg[1] == 'B')
        {
            // Binary (using custom parsing as strtol doesn't support binary)
            number = strtol(arg + 2, &endptr, 2);
        }
        else
        {
            // Assume octal
            number = strtol(arg, &endptr, 8);
        }
    }
    else
    {
        // Decimal
        number = strtol(arg, &endptr, 10);
    }

    if (endptr == arg || *endptr != '\0')
    {
        fprintf(stderr, "Invalid number: %s\n", arg);
        exit(EXIT_FAILURE);
    }

    if (errno == ERANGE)
    {
        fprintf(stderr, "Number out of range: %s\n", arg);
        exit(EXIT_FAILURE);
    }

    return number;
}


// Function to print bits from a number within a specified range [start, end]
void printBitsSection(unsigned int num, int start, int end) {
    // Check if the start and end positions are valid
    if (start > end || start < 0 || end >= (sizeof(unsigned int) * 8)) {
        printf("Invalid range.\n");
        return;
    }

    // Iterate over the specified bit range and print each bit
    for (int i = end; i >= start; i--) {
        unsigned int mask = 1 << i;
        unsigned int bit = (num & mask) >> i;
        printf("%u", bit);
    }
}

void print_r_instruction(long number)
{
    int opcode = (number >> 26) & 0x3F;
    int rs = (number >> 21) & 0x1F;
    int rt = (number >> 16) & 0x1F;
    int rd = (number >> 11) & 0x1F;
    int shamt = (number >> 6) & 0x1F;
    int funct = number & 0x3F;

    printf("R-Type Instruction:\n");

    printBitsSection(number, 26, 31);
    printf(" ");
    printBitsSection(number, 21, 25);
    printf(" ");
    printBitsSection(number, 16, 20);
    printf(" ");
    printBitsSection(number, 11, 15);
    printf(" ");
    printBitsSection(number, 6, 10);
    printf(" ");
    printBitsSection(number, 0, 5);
    printf("\n");

    printf("Opcode: %d (0x%x), Rs: %d (0x%x), Rt: %d (0x%x), Rd: %d (0x%x), Shamt: %d (0x%x), Funct: %d (0x%x)\n",
           opcode, opcode, rs, rs, rt, rt, rd, rd, shamt, shamt, funct, funct);
}

void print_i_instruction(long number)
{
    int opcode = (number >> 26) & 0x3F;
    int rs = (number >> 21) & 0x1F;
    int rt = (number >> 16) & 0x1F;
    int imm = number & 0xFFFF;

    printf("I-Type Instruction:\n");

    printBitsSection(number, 26, 31);
    printf(" ");
    printBitsSection(number, 21, 25);
    printf(" ");
    printBitsSection(number, 16, 20);
    printf(" ");
    printBitsSection(number, 0, 15);
    printf("\n");

    printf("Opcode: %d (0x%x), Rs: %d (0x%x), Rt: %d (0x%x), Imm: %d (0x%x)\n",
           opcode, opcode, rs, rs, rt, rt, imm, imm);
}

void print_j_instruction(long number)
{
    int opcode = (number >> 26) & 0x3F;
    int addr = number & 0x3FFFFFF;

    printf("J-Type Instruction:\n");

    printBitsSection(number, 26, 31);
    printf(" ");
    printBitsSection(number, 0, 25);
    printf("\n");

    printf("Opcode: %d (0x%x), Addr: %d (0x%x)\n",
           opcode, opcode, addr, addr);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <instruction type, (r or i or j)> <number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *instruction_type = argv[1];
    long number = parse_number(argv[2]);

    switch (instruction_type[0])
    {
    case 'r':
    case 'R':
        print_r_instruction(number);
        break;
    case 'i':
    case 'I':
        print_i_instruction(number);
        break;
    case 'j':
    case 'J':
        print_j_instruction(number);
        break;
    default:
        break;
    }

    return EXIT_SUCCESS;
}
