#pragma once

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKENS 100

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_REGISTER,
    TOKEN_ZERO,
    TOKEN_DEC_CONST,
    TOKEN_HEX_CONST,
    TOKEN_COMMA,
    TOKEN_L_PAREN,
    TOKEN_R_PAREN,
    TOKEN_ERROR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

/// @brief Tokenizes the input string into an array of tokens.
/// @param input 
/// @param tokens 
/// @param num_tokens 
/// @return 
void tokenize(const char *input, Token *tokens, int *num_tokens);