#pragma once

#include "tokenizer.h"

/// @brief Enumerates the different types of nodes in the AST
typedef enum {
    NODE_R_TYPE,
    NODE_I_TYPE,
    NODE_J_TYPE
} NodeType;

/// @brief Represents an R-type instruction node in the AST
typedef struct {
    Token op;
    Token rd;
    Token rs;
    Token rt;
    Token shamt;
    Token funct;
} RTypeNode;

/// @brief Represents an I-type instruction node in the AST
typedef struct {
    Token op;
    Token rs;
    Token rt;
    Token immediate;
} ITypeNode;

/// @brief Represents a J-type instruction node in the AST
typedef struct {
    Token op;
    Token address;
} JTypeNode;

/// @brief Represents a node in the AST
typedef struct {
    NodeType type;
    union {
        RTypeNode rtype;
        ITypeNode itype;
        JTypeNode jtype;
    } data;
} ASTNode;

/// @brief Represents the AST
typedef struct {
    ASTNode nodes[MAX_TOKENS]; // Adjust size as needed
    int size;
} AST;

/// @brief Parses the tokens into an AST
/// @param tokens 
/// @param num_tokens 
/// @param ast 
void parse_tokens(const Token *tokens, int num_tokens, AST *ast);