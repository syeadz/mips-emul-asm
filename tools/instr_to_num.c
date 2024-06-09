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
void printBitsSection(unsigned int num, int start, int end)
{
    // Check if the start and end positions are valid
    if (start > end || start < 0 || end >= (sizeof(unsigned int) * 8))
    {
        printf("Invalid range.\n");
        return;
    }

    // Iterate over the specified bit range and print each bit
    for (int i = end; i >= start; i--)
    {
        unsigned int mask = 1 << i;
        unsigned int bit = (num & mask) >> i;
        printf("%u", bit);
    }
}

void print_r_instr(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int funct)
{
    int instr = (opcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct;
    printf("Hex: %08x\n", instr);
    printf("Binary: ");
    printBitsSection(instr, 0, 31);
    printf("\n");
}

void print_i_instr(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int immediate)
{
    int instr = (opcode << 26) | (rs << 21) | (rt << 16) | immediate;
    printf("Hex: %08x\n", instr);
    printf("Binary: ");
    printBitsSection(instr, 0, 31);
    printf("\n");
}

void print_j_instr(unsigned int opcode, unsigned int address)
{
    int instr = (opcode << 26) | address;
    printf("Hex: %08x\n", instr);
    printf("Binary: ");
    printBitsSection(instr, 0, 31);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s <instruction type (r or i or j)> <opcode> <var args depending on instruction type>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *instr_type = argv[1];

    if (instr_type[0] == 'r')
    {
        if (argc != 8)
        {
            fprintf(stderr, "Usage: %s r <opcode> <rs> <rt> <rd> <shamt> <funct>\n", argv[0]);
            return EXIT_FAILURE;
        }

        unsigned int opcode = parse_number(argv[2]);
        unsigned int rs = parse_number(argv[3]);
        unsigned int rt = parse_number(argv[4]);
        unsigned int rd = parse_number(argv[5]);
        unsigned int shamt = parse_number(argv[6]);
        unsigned int funct = parse_number(argv[7]);

        printf("Opcode: %d (0x%x), Rs: %d (0x%x), Rt: %d (0x%x), Rd: %d (0x%x), Shamt: %d (0x%x), Funct: %d (0x%x)\n",
               opcode, opcode, rs, rs, rt, rt, rd, rd, shamt, shamt, funct, funct);
        print_r_instr(opcode, rs, rt, rd, shamt, funct);
    }
    else if (instr_type[0] == 'i')
    {
        if (argc != 6)
        {
            fprintf(stderr, "Usage: %s i <opcode> <rs> <rt> <immediate>\n", argv[0]);
            return EXIT_FAILURE;
        }

        unsigned int opcode = parse_number(argv[2]);
        unsigned int rs = parse_number(argv[3]);
        unsigned int rt = parse_number(argv[4]);
        unsigned int immediate = parse_number(argv[5]);

        printf("Opcode: %d (0x%x), Rs: %d (0x%x), Rt: %d (0x%x), Imm: %d (0x%x)\n",
               opcode, opcode, rs, rs, rt, rt, immediate, immediate);
        print_i_instr(opcode, rs, rt, immediate);
    }
    else if (instr_type[0] == 'j')
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s j <opcode> <address>\n", argv[0]);
            return EXIT_FAILURE;
        }

        unsigned int opcode = parse_number(argv[2]);
        unsigned int address = parse_number(argv[3]);

        printf("Opcode: %d (0x%x), Addr: %d (0x%x)\n",
               opcode, opcode, address, address);
        print_j_instr(opcode, address);
    }
    else
    {
        fprintf(stderr, "Invalid instruction type: %s\n", instr_type);
        return EXIT_FAILURE;
    }

    /* code */
    return 0;
}
