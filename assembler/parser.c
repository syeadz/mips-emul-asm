#include "parser.h"

/// @brief Parses an R-type instruction with the format "op rd, rs, rt" into an AST node
/// @param tokens
/// @param index
/// @param ast
void parse_r_type_f_rd_rs_rt(const Token *tokens, int *index, AST *ast)
{
    ASTNode node;
    node.type = NODE_R_TYPE;
    // Get opcode and increment index
    node.data.rtype.op = tokens[(*index)++];

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }

    // Get register and increment index
    node.data.rtype.rd = tokens[(*index)++];

    // Check if next token is a comma
    if (tokens[*index].type != TOKEN_COMMA)
    {
        printf("Error: Expected comma at index %d\n", *index);
        return;
    }
    // Increment index
    (*index)++;

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }
    // Get register and increment index
    node.data.rtype.rs = tokens[(*index)++];

    // Check if next token is a comma
    if (tokens[*index].type != TOKEN_COMMA)
    {
        printf("Error: Expected comma at index %d\n", *index);
        return;
    }
    // Increment index
    (*index)++;

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }
    // Get register and increment index
    node.data.rtype.rt = tokens[(*index)++];

    // No need to check for shamt and funct for now
    node.data.rtype.shamt.type = TOKEN_ZERO;
    node.data.rtype.shamt.value = "0";

    // TODO: add function to check opcode and add correct funct
    node.data.rtype.funct.type = TOKEN_ZERO;
    node.data.rtype.funct.value = "0";

    // Add node to AST
    ast->nodes[ast->size++] = node;
}

/// @brief Parses an I-type instruction with the format "op rt, i(rs) into an AST node
/// @param tokens
/// @param index
/// @param ast
void parse_i_type_rt_i_rs(const Token *tokens, int *index, AST *ast)
{
    ASTNode node;
    node.type = NODE_I_TYPE;
    // Get opcode and increment index
    node.data.itype.op = tokens[(*index)++];

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }
    // Get register and increment index
    node.data.itype.rt = tokens[(*index)++];

    // Check if next token is a comma
    if (tokens[*index].type != TOKEN_COMMA)
    {
        printf("Error: Expected comma at index %d\n", *index);
        return;
    }
    // Increment index
    (*index)++;

    // Check if next token is a decimal or hexadecimal constant
    if (tokens[*index].type != TOKEN_DEC_CONST && tokens[*index].type != TOKEN_HEX_CONST && tokens[*index].type != TOKEN_ZERO)
    {
        printf("Error: Expected immediate at index %d\n", *index);
        return;
    }
    // Get immediate and increment index
    node.data.itype.immediate = tokens[(*index)++];

    // Check if next token is a left parenthesis
    if (tokens[*index].type != TOKEN_L_PAREN)
    {
        printf("Error: Expected left parenthesis\n");
        return;
    }
    // Increment index
    (*index)++;

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }
    // Get register and increment index
    node.data.itype.rs = tokens[(*index)++];

    // Check if next token is a right parenthesis
    if (tokens[*index].type != TOKEN_R_PAREN)
    {
        printf("Error: Expected right parenthesis\n");
        return;
    }
    // Increment index
    (*index)++;

    // Add node to AST
    ast->nodes[ast->size++] = node;
}

/// @brief Parses an R-type instruction with the format "op rs, rt, label" into an AST node
/// @param tokens
/// @param index
/// @param ast
void parse_i_type_rs_rt_label(const Token *tokens, int *index, AST *ast)
{
    ASTNode node;
    node.type = NODE_I_TYPE;
    // Get opcode and increment index
    node.data.itype.op = tokens[(*index)++];

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }
    // Get register and increment index
    node.data.itype.rs = tokens[(*index)++];

    // Check if next token is a comma
    if (tokens[*index].type != TOKEN_COMMA)
    {
        printf("Error: Expected comma at index %d\n", *index);
        return;
    }
    // Increment index
    (*index)++;

    // Check if next token is a register
    if (tokens[*index].type != TOKEN_REGISTER)
    {
        printf("Error: Expected register at index %d\n", *index);
        return;
    }
    // Get register and increment index
    node.data.itype.rt = tokens[(*index)++];

    // Check if next token is a comma
    if (tokens[*index].type != TOKEN_COMMA)
    {
        printf("Error: Expected comma at index %d\n", *index);
        return;
    }
    // Increment index
    (*index)++;

    // Check if next token is a decimal or hexadecimal constant
    if (tokens[*index].type != TOKEN_DEC_CONST && tokens[*index].type != TOKEN_HEX_CONST && tokens[*index].type != TOKEN_ZERO)
    {
        printf("Error: Expected immediate at index %d\n", *index);
        return;
    }
    // Get immediate and increment index
    node.data.itype.immediate = tokens[(*index)++];

    // Add node to AST
    ast->nodes[ast->size++] = node;
}

/// @brief Parses a J-type instruction with the format "op label" into an AST node
/// @param tokens
/// @param index
/// @param ast
void parse_j_type_label(const Token *tokens, int *index, AST *ast)
{
    ASTNode node;
    node.type = NODE_J_TYPE;
    // Get opcode and increment index
    node.data.jtype.op = tokens[(*index)++];

    // Check if next token is an immediate
    if (tokens[*index].type != TOKEN_DEC_CONST && tokens[*index].type != TOKEN_HEX_CONST && tokens[*index].type != TOKEN_ZERO)
    {
        printf("Error: Expected immediate at index %d\n", *index);
        return;
    }
    // Get immediate and increment index
    node.data.jtype.address = tokens[(*index)++];

    // Add node to AST
    ast->nodes[ast->size++] = node;
}

void parse_tokens(const Token *tokens, int num_tokens, AST *ast)
{
    ast->size = 0;
    int index = 0;

    while (index < num_tokens)
    {
        if (tokens[index].type == TOKEN_IDENTIFIER)
        {
            // TODO: Create function to check token instruction type
            if (strcmp(tokens[index].value, "add") == 0)
            {
                parse_r_type_f_rd_rs_rt(tokens, &index, ast);
            }
            else if (strcmp(tokens[index].value, "lw") == 0 || strcmp(tokens[index].value, "sw") == 0)
            {
                parse_i_type_rt_i_rs(tokens, &index, ast);
            }
            else if (strcmp(tokens[index].value, "beq") == 0)
            {
                parse_i_type_rs_rt_label(tokens, &index, ast);
            }
            else if (strcmp(tokens[index].value, "j") == 0)
            {
                parse_j_type_label(tokens, &index, ast);
            }
            else
            {
                printf("Error: Unknown instruction %s\n", tokens[index].value);
                return;
            }
        }
        else
        {
            printf("Error: Expected identifier at index %d\n", index);
            return;
        }
    }
}

// void print_token(const Token *token) {
//     const char *token_type_str[] = {
//         "Identifier",
//         "Register",
//         "Zero",
//         "Decimal Constant",
//         "Hexadecimal Constant",
//         "Comma",
//         "Left Parenthesis",
//         "Right Parenthesis",
//         "Error",
//         "Unknown"
//     };

//     printf("Token: %s, Type: %s\n", token->value, token_type_str[token->type]);
// }

// void print_ast(const AST *ast) {
//     for (int i = 0; i < ast->size; i++) {
//         ASTNode node = ast->nodes[i];
//         switch (node.type) {
//             case NODE_R_TYPE:
//                 printf("R-Type Instruction: %s %s, %s, %s\n",
//                     node.data.rtype.op.value,
//                     node.data.rtype.rd.value,
//                     node.data.rtype.rs.value,
//                     node.data.rtype.rt.value);
//                 break;
//             case NODE_I_TYPE:
//                 printf("I-Type Instruction: %s %s, %s, %s\n",
//                     node.data.itype.op.value,
//                     node.data.itype.rt.value,
//                     node.data.itype.rs.value,
//                     node.data.itype.immediate.value);
//                 break;
//             case NODE_J_TYPE:
//                 printf("J-Type Instruction: %s %s\n",
//                     node.data.jtype.op.value,
//                     node.data.jtype.address.value);
//                 break;
//             default:
//                 printf("Unknown Node Type\n");
//                 break;
//         }
//     }
// }

// int main() {
//     const char *input = "add $t0, $t1, $t2\n"
//                         "lw $t6, 0($t7)\n"
//                         "sw $t8, 4($t9)\n"
//                         "beq $t3, $t4, 100\n"
//                         "j 1000\n";

//     Token tokens[MAX_TOKENS];
//     int num_tokens = 0;

//     tokenize(input, tokens, &num_tokens);

//     printf("Number of tokens: %d\n", num_tokens);

//     AST ast;
//     parse_tokens(tokens, num_tokens, &ast);
//     print_ast(&ast);

//     return 0;
// }