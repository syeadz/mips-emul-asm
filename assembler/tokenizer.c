#include "tokenizer.h"

/// @brief Enumerates the possible states of the DFA. Also used to classify the token.
typedef enum
{
    START,
    WHITESPACE,
    INSTRUCTION,
    LABEL_INTER,
    LABEL,
    DEC_CONST,
    REGISTER_PREFIX,
    REGISTER_INTER,
    REGISTER,
    REGISTER_ZERO_Z,
    REGISTER_ZERO_E,
    REGISTER_ZERO_R,
    ZERO,
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
        if (isalpha(input))
            return INSTRUCTION;

        if (isdigit(input))
        {
            if (input == '0')
                return ZERO;
            return DEC_CONST;
        }

        return ERROR;

    case COMMENT:
        if (input != '\n')
            return COMMENT;
        return START; // Newline detected, return to START state

    // WHITESPACE state is used to skip over whitespace characters
    case WHITESPACE:
        if (isspace(input))
            return WHITESPACE;
        return START;

    case COMMA:
    case L_PAREN:
    case R_PAREN:
        return START;

    // INSTRUCTION state is used to classify identifiers, either labels or instructions
    case INSTRUCTION:
        if (input == ':')
            return LABEL;
        if (isdigit(input))
            return LABEL_INTER;
        if (isalpha(input))
            return INSTRUCTION;
        return START;

    case LABEL_INTER:
        if (input == ':')
            return LABEL;
        if (isalnum(input))
            return LABEL_INTER;
        return ERROR;

    case LABEL:
        return START;

    // REGISTER_PREFIX means $ and the next character should be a valid register name, minus zero (t, s, a, v)
    case REGISTER_PREFIX:
        if (input == 't' || input == 's' || input == 'a' || input == 'v')
            return REGISTER_INTER;
        if (input == 'z')
            return REGISTER_ZERO_Z;
        return ERROR;

    case REGISTER_INTER:
        if (isdigit(input))
            return REGISTER;
        return ERROR;

    // REGISTER state is used to classify register names, which are $ followed by a letter and a digit
    case REGISTER:
        return START;

    case REGISTER_ZERO_Z:
        if (input == 'e')
            return REGISTER_ZERO_E;
        return ERROR;

    // Special cases for the zero register
    case REGISTER_ZERO_E:
        if (input == 'r')
            return REGISTER_ZERO_R;
        return ERROR;

    case REGISTER_ZERO_R:
        if (input == 'o')
            return REGISTER;
        return ERROR;

    // ZERO state is used to classify the number 0, which can be used to represent a zero constant or a hexadecimal constant
    case ZERO:
        if (input == 'x' || input == 'X')
            return HEX_PREFIX;
        if (isdigit(input))
            return ERROR;
        return START;

    // DEC_CONST state is used to classify decimal constants
    case DEC_CONST:
        if (isdigit(input))
            return DEC_CONST;
        return START;

    // HEX_PREFIX state is used to classify hexadecimal constants, it is the x or X after a 0
    case HEX_PREFIX:
        if (isxdigit(input))
            return HEX_CONST;
        return ERROR;

    // HEX_CONST state is used to classify hexadecimal constants
    case HEX_CONST:
        if (isxdigit(input))
            return HEX_CONST;
        return START;

    default:
        return ERROR;
    }
}

int tokenize(const char *input, Token *tokens, int *num_tokens)
{
    // We start in the START state with the input pointer at the beginning of the input string
    State state = START;
    const char *token_start = input;

    // While we haven't reached the end of the input string
    while (*input)
    {
        // Determine the next state based on the current state and the current input character
        State next = next_state(state, *input);
        if (next == ERROR)
        {
            fprintf(stderr, "Error: Invalid character '%c' at token %d\n", *input, *num_tokens);
            return 1; // Return 1 to indicate an error
        }
        else if (next == START && state != START)
        {
            if (state != COMMENT)
            {
                tokens[*num_tokens].type = state_to_token_type(state);
                tokens[*num_tokens].value = strndup(token_start, input - token_start);
                // printf("Token %d: %.*s, Type: %s\n", *num_tokens, (int)(input - token_start), token_start, state_to_str(state));
                (*num_tokens)++;
            }

            while (isspace(*input))
            {
                input++;
            }

            token_start = input;
            state = START;
        }
        else
        {
            state = next;
            input++;

            if (state == WHITESPACE)
            {
                token_start = input;
            }
        }
    }

    // Last token
    if (state != START && state != WHITESPACE)
    {
        tokens[*num_tokens].type = state_to_token_type(state);
        tokens[*num_tokens].value = strndup(token_start, input - token_start);
        (*num_tokens)++;
        printf("Token %d: %.*s, Type: %s\n", *num_tokens, (int)(input - token_start), token_start, state_to_str(state));
    }

    return 0; // Return 0 to indicate success
}

/// @brief Converts a state to a string for debugging purposes.
/// @param state
/// @return
const char *state_to_str(State state)
{
    switch (state)
    {
    case START:
        return "Start";
    case WHITESPACE:
        return "Whitespace";
    case INSTRUCTION:
        return "Instruction";
    case LABEL_INTER:
        return "Label Intermediate";
    case LABEL:
        return "Label";
    case DEC_CONST:
        return "Decimal Constant";
    case REGISTER_PREFIX:
        return "Register Prefix";
    case REGISTER_INTER:
        return "Register Intermediate";
    case REGISTER:
        return "Register";
    case REGISTER_ZERO_Z:
        return "Register Zero Z";
    case REGISTER_ZERO_E:
        return "Register Zero E";
    case REGISTER_ZERO_R:
        return "Register Zero R";
    case ZERO:
        return "Zero";
    case HEX_PREFIX:
        return "Hex Prefix";
    case HEX_CONST:
        return "Hex Constant";
    case COMMA:
        return "Comma";
    case L_PAREN:
        return "Left Parenthesis";
    case R_PAREN:
        return "Right Parenthesis";
    case COMMENT:
        return "Comment";
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
    case INSTRUCTION:
        return TOKEN_INSTRUCTION;
    case LABEL:
        return TOKEN_LABEL;
    case REGISTER:
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