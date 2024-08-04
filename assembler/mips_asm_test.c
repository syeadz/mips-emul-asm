#include "../minunit.h"
#include "mips_asm.h"

// Function prototypes
void token_add_instr_rg_off_rg(Token tokens[], int *count, char *instr, char *reg1, char *offset, char *reg2);
void token_add_instr_rg_rg_rg(Token tokens[], int *count, char *instr, char *reg1, char *reg2, char *reg3);
void token_add_instr_rg_rg_const(Token tokens[], int *count, char *instr, char *reg1, char *reg2, char *const_val);

// ********* function tests ********* //

MU_TEST(test_add_asm)
{
    char *code = "lw $t1, 48($zero)\n"
                 "lw $t2, 52($zero)\n"
                 "# ignore my comment\n"
                 "add $t0, $t1, $t2\n"
                 "sw $t0, 56($zero)\n"
                 "lw $t3, 56($zero)\n"
                 "#ignore my comment again\n"
                 "beq $t0, $t3, 9\n";

    Token tokens[MAX_TOKENS];
    int token_count = 0;

    tokenize(code, tokens, &token_count);

    mu_assert(token_count == 40, "Token count wrong");

    Token correct_tokens[40];
    int count = 0;
    token_add_instr_rg_off_rg(correct_tokens, &count, "lw", "$t1", "48", "$zero");
    token_add_instr_rg_off_rg(correct_tokens, &count, "lw", "$t2", "52", "$zero");
    token_add_instr_rg_rg_rg(correct_tokens, &count, "add", "$t0", "$t1", "$t2");
    token_add_instr_rg_off_rg(correct_tokens, &count, "sw", "$t0", "56", "$zero");
    token_add_instr_rg_off_rg(correct_tokens, &count, "lw", "$t3", "56", "$zero");
    token_add_instr_rg_rg_const(correct_tokens, &count, "beq", "$t0", "$t3", "9");

    char error_msg[50];

    for (size_t i = 0; i < 40; i++)
    {
        TokenType type = tokens[i].type;
        char *value = tokens[i].value;

        TokenType cor_type = correct_tokens[i].type;
        char *cor_value = correct_tokens[i].value;

        // Currently returning struct integer rather than string
        sprintf(error_msg, "Type error at token %ld: %d, should be: %d", i, type, cor_type);
        mu_assert(cor_type == type, error_msg);

        sprintf(error_msg, "Value error at token %ld: %s, should be: %s", i, value, cor_value);
        mu_assert(!strcmp(cor_value, value), error_msg);
    }
}

MU_TEST_SUITE(tokenizer_tests)
{
    MU_RUN_TEST(test_add_asm);
}

int main()
{
    MU_RUN_SUITE(tokenizer_tests);

    MU_REPORT();
    return MU_EXIT_CODE;
}

// ********* Helper functions ********* //

TokenType get_const_token_type(char *const_val)
{
    if (strlen(const_val) < 2 || const_val[0] != 0)
    {
        return TOKEN_DEC_CONST;
    }

    if (const_val[1] == 'x' || const_val[1] == 'X')
    {
        return TOKEN_HEX_CONST;
    }

    return TOKEN_ERROR;
}

/// Adds proper tokens to array for a lw $reg, const($reg) type instruction
void token_add_instr_rg_off_rg(Token tokens[], int *count, char *instr, char *reg1, char *offset, char *reg2)
{
    tokens[(*count)++] = (Token){.type = TOKEN_IDENTIFIER, .value = instr};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, .value = reg1};
    tokens[(*count)++] = (Token){.type = TOKEN_COMMA, .value = ","};
    tokens[(*count)++] = (Token){.type = get_const_token_type(offset), offset};
    tokens[(*count)++] = (Token){.type = TOKEN_L_PAREN, "("};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, reg2};
    tokens[(*count)++] = (Token){.type = TOKEN_R_PAREN, ")"};
}

/// Adds proper tokens to array for an add $reg1, $reg2, $reg3 type instruction
void token_add_instr_rg_rg_rg(Token tokens[], int *count, char *instr, char *reg1, char *reg2, char *reg3)
{
    tokens[(*count)++] = (Token){.type = TOKEN_IDENTIFIER, .value = instr};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, .value = reg1};
    tokens[(*count)++] = (Token){.type = TOKEN_COMMA, .value = ","};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, .value = reg2};
    tokens[(*count)++] = (Token){.type = TOKEN_COMMA, .value = ","};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, .value = reg3};
}

/// Adds proper tokens to array for an add $reg1, $reg2, dec_const type instruction
void token_add_instr_rg_rg_const(Token tokens[], int *count, char *instr, char *reg1, char *reg2, char *const_val)
{
    tokens[(*count)++] = (Token){.type = TOKEN_IDENTIFIER, .value = instr};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, .value = reg1};
    tokens[(*count)++] = (Token){.type = TOKEN_COMMA, .value = ","};
    tokens[(*count)++] = (Token){.type = TOKEN_REGISTER, .value = reg2};
    tokens[(*count)++] = (Token){.type = TOKEN_COMMA, .value = ","};
    tokens[(*count)++] = (Token){.type = get_const_token_type(const_val), .value = const_val};
}
