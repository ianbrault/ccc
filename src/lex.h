/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef LEX_H
#define LEX_H

#include <stdint.h>

// temporary max length of input string
#define MAX_INPUT_LEN (1024)
// temporary max number of tokens
#define MAX_TOKENS (256)

typedef enum {
    INVALID,
    LITERAL,
    L_PAREN,
    R_PAREN,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_POS,
    OP_NEG,
    N_TOKEN_TYPES
} token_type;

#define N_BINARY_OPS 3
#define N_UNARY_OPS  2

#define IS_LITERAL(token)  ((token).type == LITERAL)
#define IS_OPERATOR(token) ((token).type >= OP_ADD && (token).type <= OP_NEG)

// arity of operators
static uint8_t arity[N_TOKEN_TYPES] = {
    [INVALID] = 0,
    [LITERAL] = 0,
    [L_PAREN] = 0,
    [R_PAREN] = 0,
    [OP_ADD]  = 2,
    [OP_SUB]  = 2,
    [OP_MUL]  = 2,
    [OP_POS]  = 1,
    [OP_NEG]  = 1,
};

#define ARITY(token) arity[(token).type]

// precedence of operators, taken from "C Operator Precedence"
// https://en.cppreference.com/w/c/language/operator_precedence
static uint8_t precedence[N_TOKEN_TYPES] = {
    [INVALID] = 0,
    [LITERAL] = 0,
    [L_PAREN] = 1,
    [R_PAREN] = 1,
    [OP_ADD]  = 4,
    [OP_SUB]  = 4,
    [OP_MUL]  = 3,
    [OP_POS]  = 2,
    [OP_NEG]  = 2,
};

#define PREC(token) precedence[(token).type]
#define PREC_GT(token1, token2) PREC(token1) < PREC(token2)
#define PREC_EQ(token1, token2) PREC(token1) == PREC(token2)

#define ASSOC_L 1
#define ASSOC_R 2

// associativity of operators
static uint8_t associativity[N_TOKEN_TYPES] = {
    [INVALID] = 0,
    [LITERAL] = 0,
    [L_PAREN] = 0,
    [R_PAREN] = 0,
    [OP_ADD]  = ASSOC_L,
    [OP_SUB]  = ASSOC_L,
    [OP_MUL]  = ASSOC_L,
    [OP_POS]  = ASSOC_R,
    [OP_NEG]  = ASSOC_R,
};

#define ASSOC(token) associativity[(token).type]

typedef struct {
    token_type type;
    int32_t value;   // only used for literals
    int32_t offset;  // offset from start of input string, used for errors
} token_t;

/*
 * initialize a token (not a literal) with a given type and offset
 *
 * @oparam token := token struct to be initialized
 * @iparam type := token_type for the token
 * @iparam offset := offset into the input string
 */
void init_token(token_t* token, token_type type, int32_t offset);

/*
 * initialize a literal with a given value and offset
 *
 * @oparam token := token struct to be initialized
 * @iparam value := integer value for the literal
 * @iparam offset := offset into the input string
 */
void init_literal(token_t* token, int32_t value, int32_t offset);

/*
 * splits an input string into tokens
 *
 * @iparam input := input string
 * @oparam n_tokens := length of the returned array
 * @returns an array of tokens
 */
token_t* tokenize(const char* input, int32_t* n_tokens);

#endif
