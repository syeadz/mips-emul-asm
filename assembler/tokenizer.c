#include "tokenizer.h"

/// @brief Enumerates the possible states of the DFA. Also used to classify the token.
typedef enum
{
    START,
    WHITESPACE,
    IDENTIFIER,
    INSTRUCTION,
    LABEL,
    REGISTER_PREFIX,
    REGISTER,
    REGISTER_ZERO_E,
    REGISTER_ZERO_R,
    REGISTER_ZERO_0,
    ZERO,
    DEC_CONST,
    HEX_PREFIX,
    HEX_CONST,
    COMMA,
    L_PAREN,
    R_PAREN,
    COMMENT,
    ERROR,
} State;

// Function prototypes
const char *state_to_str(State state);
TokenType state_to_token_type(State state);

// Function to determine the next state
State next_state(State current, char input)
{
    switch (current)
    {
    // Transition rules

    // START state is the initial state, where the DFA starts processing the input
    case START:
        if (input == '#')
            return COMMENT;
        if (isalpha(input))
            return IDENTIFIER;

        if (isdigit(input))
        {
            if (input == '0')
                return ZERO;
            return DEC_CONST;
        }

        if (input == ',')
            return COMMA;
        if (input == '(')
            return L_PAREN;
        if (input == ')')
            return R_PAREN;
        if (input == '$')
            return REGISTER_PREFIX;
        if (isspace(input))
            return WHITESPACE;

        return ERROR;

    case COMMENT:
        if (input != '\n')
            return COMMENT;
        break;

    // WHITESPACE state is used to skip over whitespace characters
    case WHITESPACE:
        if (isspace(input))
            return WHITESPACE;
        break;

    // IDENTIFIER state is used to classify identifiers, which are sequences of letters
    case IDENTIFIER:
        if (isalnum(input))
            return IDENTIFIER;
        break;

    // REGISTER_PREFIX means $ and the next character should be a valid register name (t, s, a, v, z)
    case REGISTER_PREFIX:
        if (input == 't' || input == 's' || input == 'a' || input == 'v' || input == 'z')
            return REGISTER;
        break;

    // REGISTER state is used to classify register names, which are $ followed by a letter and a digit
    case REGISTER:
        if (isdigit(input))
            return REGISTER;
        // Special case for the zero register
        if (input == 'e')
            return REGISTER_ZERO_E;
        break;

    // Special cases for the zero register
    case REGISTER_ZERO_E:
        if (input == 'r')
            return REGISTER_ZERO_R;
        break;

    case REGISTER_ZERO_R:
        if (input == 'o')
            return REGISTER_ZERO_0;
        break;

    // ZERO state is used to classify the number 0, which can be used to represent a zero constant or a hexadecimal constant
    case ZERO:
        if (input == 'x' || input == 'X')
            return HEX_PREFIX;
        if (isdigit(input))
            return DEC_CONST;
        break;

    // DEC_CONST state is used to classify decimal constants
    case DEC_CONST:
        if (isdigit(input))
            return DEC_CONST;
        break;

    // HEX_PREFIX state is used to classify hexadecimal constants, it is the x or X after a 0
    case HEX_PREFIX:
        if (isxdigit(input))
            return HEX_CONST;
        break;

    // HEX_CONST state is used to classify hexadecimal constants
    case HEX_CONST:
        if (isxdigit(input))
            return HEX_CONST;
        break;
    default:
        return ERROR;
    }
    return ERROR;
}

void tokenize(const char *input, Token *tokens, int *num_tokens)
{
    // We start in the START state with the input pointer at the beginning of the input string
    State state = START;
    const char *token_start = input;

    int token_num = 0;

    // While we haven't reached the end of the input string
    while (*input)
    {
        // Determine the next state based on the current state and the current input character
        State next = next_state(state, *input);
        // If the next state is WHITESPACE, ERROR or START, we have reached the end of the current token
        if (next == WHITESPACE || next == ERROR || next == START)
        {
            // If the current state is not START, WHITESPACE, COMMENT, or ERROR, we have a valid token
            if (state != START && state != WHITESPACE && state != COMMENT)
            {
                tokens[*num_tokens].type = state_to_token_type(state);
                tokens[*num_tokens].value = strndup(token_start, input - token_start);
                (*num_tokens)++;
                // printf("Token %d: %.*s, Type: %s\n", token_num, (int)(input - token_start), token_start, state_to_str(state));
                token_num++;
            }
            // Update the token start pointer and the state
            token_start = input + (next == WHITESPACE ? 1 : 0);
            // We increment the input pointer if the next state is WHITESPACE
            input += (next == WHITESPACE ? 1 : 0);
            state = START;
        }
        else
        {
            state = next;
            input++;
        }
    }

    // Last token
    if (state != START && state != ERROR && state != WHITESPACE)
    {
        tokens[*num_tokens].type = state_to_token_type(state);
        tokens[*num_tokens].value = strndup(token_start, input - token_start);
        (*num_tokens)++;
        // printf("Token %d: %.*s, Type: %s\n", token_num, (int)(input - token_start), token_start, state_to_str(state));
    }
}

/// @brief Converts a state to a string for debugging purposes.
/// @param state
/// @return
const char *state_to_str(State state)
{
    switch (state)
    {
    case IDENTIFIER:
        return "Identifier";
    case INSTRUCTION:
        return "Instruction";
    case LABEL:
        return "Label";
    case REGISTER:
        return "Register";
    case REGISTER_ZERO_0:
        return "Register";
    case ZERO:
        return "Zero";
    case DEC_CONST:
        return "Decimal Constant";
    case HEX_CONST:
        return "Hexadecimal Constant";
    case COMMA:
        return "Comma";
    case L_PAREN:
        return "Left Parenthesis";
    case R_PAREN:
        return "Right Parenthesis";
    default:
        return "Error";
    }
}

/// @brief Converts a state to a token type.
/// @param state
/// @return
TokenType state_to_token_type(State state)
{
    switch (state)
    {
    case IDENTIFIER:
        return TOKEN_INSTRUCTION;
    case REGISTER:
        return TOKEN_REGISTER;
    case REGISTER_ZERO_0:
        return TOKEN_REGISTER;
    case ZERO:
        return TOKEN_ZERO;
    case DEC_CONST:
        return TOKEN_DEC_CONST;
    case HEX_CONST:
        return TOKEN_HEX_CONST;
    case COMMA:
        return TOKEN_COMMA;
    case L_PAREN:
        return TOKEN_L_PAREN;
    case R_PAREN:
        return TOKEN_R_PAREN;
    default:
        return TOKEN_ERROR;
    }
}