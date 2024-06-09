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
            int opcode = get_opcode(node.data.rtype.op.value);
            int rd = get_register_number(node.data.rtype.rd.value);
            int rs = get_register_number(node.data.rtype.rs.value);
            int rt = get_register_number(node.data.rtype.rt.value);
            int shamt = 0;
            int funct = 0;
            funct = 0x20; // hard coded to add for now
            // TODO: implement funct

            instruction = (opcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | funct;
            break;
        case NODE_I_TYPE:
            opcode = get_opcode(node.data.itype.op.value);
            rs = get_register_number(node.data.itype.rs.value);
            rt = get_register_number(node.data.itype.rt.value);
            int immediate = atoi(node.data.itype.immediate.value);

            instruction = (opcode << 26) | (rs << 21) | (rt << 16) | immediate;
            break;
        case NODE_J_TYPE:
            opcode = get_opcode(node.data.jtype.op.value);
            int address = atoi(node.data.jtype.address.value);

            instruction = (opcode << 26) | address;
            break;
        }

        print_binary(file, instruction);
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    Token tokens[MAX_TOKENS];
    int token_count = 0;
    AST program;

    char *input = read_file(input_filename);

    tokenize(input, tokens, &token_count);
    parse_tokens(tokens, token_count, &program);
    generate_code(&program, output_filename);

    // Free allocated memory.
    free(input);
    // Free other allocated structures as necessary.

    return EXIT_SUCCESS;
}
