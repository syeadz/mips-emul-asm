#include "mips_asm.h"

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

    return EXIT_SUCCESS;
}