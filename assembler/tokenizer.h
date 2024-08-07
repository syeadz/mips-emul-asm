#pragma once

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKENS 100

typedef enum
{
    TOKEN_INSTRUCTION,
    TOKEN_LABEL,
    TOKEN_REGISTER,
    TOKEN_DIRECTIVE,
    TOKEN_STRING,
    TOKEN_ZERO,
    TOKEN_DEC_CONST,
    TOKEN_HEX_CONST,
    TOKEN_COMMA,
    TOKEN_L_PAREN,
    TOKEN_R_PAREN,
    TOKEN_ERROR,
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

/// @brief Tokenizes the input string into an array of tokens. Returns 0 if successful, 1 otherwise.
/// @param input
/// @param tokens
/// @param num_tokens
/// @return 0 if successful, 1 otherwise
int tokenize(const char *input, Token *tokens, int *num_tokens);