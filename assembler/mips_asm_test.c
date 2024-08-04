#include "../minunit.h"
#include "mips_asm.h"

// ********* function tests ********* //

MU_TEST(test_add_asm)
{
    char *code = "lw $t1, 48($zero)"
                 "lw $t2, 52($zero)"
                 "add $t0, $t1, $t2"
                 "sw $t0, 56($zero)"
                 "lw $t3, 56($zero)"
                 "beq $t0, $t3, 9";

    Token tokens[MAX_TOKENS];
    int token_count = 0;

    tokenize(code, tokens, &token_count);

    mu_assert(token_count == 40, "Token count wrong");

    Token correct_tokens[] = {
        [0] = {.type = TOKEN_IDENTIFIER, .value = "lw"},
        [1] = {.type = TOKEN_REGISTER, .value = "$t1"},
        [2] = {.type = TOKEN_COMMA, .value = ","},
        [3] = {.type = TOKEN_DEC_CONST, .value = "48"},
        [4] = {.type = TOKEN_L_PAREN, .value = "("},
        [5] = {.type = TOKEN_REGISTER, .value = "$zero"},
        [6] = {.type = TOKEN_R_PAREN, .value = ")"},
        [7] = {.type = TOKEN_IDENTIFIER, .value = "lw"},
        [8] = {.type = TOKEN_REGISTER, .value = "$t2"},
        [9] = {.type = TOKEN_COMMA, .value = ","},
        [10] = {.type = TOKEN_DEC_CONST, .value = "52"},
        [11] = {.type = TOKEN_L_PAREN, .value = "("},
        [12] = {.type = TOKEN_REGISTER, .value = "$zero"},
        [13] = {.type = TOKEN_R_PAREN, .value = ")"},
        [14] = {.type = TOKEN_IDENTIFIER, .value = "add"},
        [15] = {.type = TOKEN_REGISTER, .value = "$t0"},
        [16] = {.type = TOKEN_COMMA, .value = ","},
        [17] = {.type = TOKEN_REGISTER, .value = "$t1"},
        [18] = {.type = TOKEN_COMMA, .value = ","},
        [19] = {.type = TOKEN_REGISTER, .value = "$t2"},
        [20] = {.type = TOKEN_IDENTIFIER, .value = "sw"},
        [21] = {.type = TOKEN_REGISTER, .value = "$t0"},
        [22] = {.type = TOKEN_COMMA, .value = ","},
        [23] = {.type = TOKEN_DEC_CONST, .value = "56"},
        [24] = {.type = TOKEN_L_PAREN, .value = "("},
        [25] = {.type = TOKEN_REGISTER, .value = "$zero"},
        [26] = {.type = TOKEN_R_PAREN, .value = ")"},
        [27] = {.type = TOKEN_IDENTIFIER, .value = "lw"},
        [28] = {.type = TOKEN_REGISTER, .value = "$t3"},
        [29] = {.type = TOKEN_COMMA, .value = ","},
        [30] = {.type = TOKEN_DEC_CONST, .value = "56"},
        [31] = {.type = TOKEN_L_PAREN, .value = "("},
        [32] = {.type = TOKEN_REGISTER, .value = "$zero"},
        [33] = {.type = TOKEN_R_PAREN, .value = ")"},
        [34] = {.type = TOKEN_IDENTIFIER, .value = "beq"},
        [35] = {.type = TOKEN_REGISTER, .value = "$t0"},
        [36] = {.type = TOKEN_COMMA, .value = ","},
        [37] = {.type = TOKEN_REGISTER, .value = "$t3"},
        [38] = {.type = TOKEN_COMMA, .value = ","},
        [39] = {.type = TOKEN_DEC_CONST, .value = "9"},
    };

    char error_msg[50];

    for (size_t i = 0; i < token_count; i++)
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