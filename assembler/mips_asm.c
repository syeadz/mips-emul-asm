#include "mips_asm.h"

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

// This will print the binary representation of the instruction but as a string of 1s and 0s
void print_binary(FILE *file, uint32_t instruction)
{
    for (int i = 31; i >= 0; i--)
    {
        fprintf(file, "%d", (instruction >> i) & 1);
        // if (i % 4 == 0)
        //     fprintf(file, " "); // Add space for readability TODO remove later
    }
    fprintf(file, "\n");
}

// This will write the binary representation of the instruction to the file
void write_binary(FILE *file, uint32_t instruction)
{
    // Convert to big-endian before writing NOTE: VERY IMPORTANT!!!
    uint32_t big_endian_instruction = htonl(instruction);
    fwrite(&big_endian_instruction, sizeof(uint32_t), 1, file);
}

void generate_code(AST *ast, const char *output_filename)
{
    FILE *file = fopen(output_filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", output_filename);
        exit(EXIT_FAILURE);
    }

    uint32_t instruction;

    for (int i = 0; i < ast->size; i++)
    {
        ASTNode node = ast->nodes[i];
        switch (node.type)
        {
            // TODO: have a case for each format type (op rs rs rt vs op rs rt)
        case NODE_R_TYPE:
            uint8_t opcode = get_opcode_from_mnemonic(node.data.rtype.op.value);
            int rd = get_register_number_from_name(node.data.rtype.rd.value);
            int rs = get_register_number_from_name(node.data.rtype.rs.value);
            int rt = get_register_number_from_name(node.data.rtype.rt.value);
            int shamt = 0;
            int funct = 0;
            funct = 0x20; // hard coded to add for now
            // TODO: implement funct

            instruction = (opcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct;
            break;
        case NODE_I_TYPE:
            opcode = get_opcode_from_mnemonic(node.data.itype.op.value);
            rs = get_register_number_from_name(node.data.itype.rs.value);
            rt = get_register_number_from_name(node.data.itype.rt.value);
            int immediate = atoi(node.data.itype.immediate.value);

            instruction = (opcode << 26) | (rs << 21) | (rt << 16) | immediate;
            break;
        case NODE_J_TYPE:
            opcode = get_opcode_from_mnemonic(node.data.jtype.op.value);
            int address = atoi(node.data.jtype.address.value);

            instruction = (opcode << 26) | address;
            break;
        }

        write_binary(file, instruction);
    }
    fclose(file);
}
