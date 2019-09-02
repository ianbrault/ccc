/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef LEX_H
#define LEX_H

#include <stdint.h>

typedef enum {
    INVALID,
    LITERAL,
    L_PAREN,
    R_PAREN,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    N_TOKEN_TYPES
} token_type;

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
};

#define PREC(token) precedence[token.type]
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
};

#define ASSOC(token) associativity[token.type]

typedef struct {
    token_type type;
    int32_t value;   // only used for literals
    int32_t offset;  // offset from start of input string, used for errors
} token_t;

// FIXME: temporary max number of tokens
// implement to use a resizing scheme later on
#define MAX_TOKENS (UINT8_MAX)

/*
 * returns a boolean to indicate whether the token type is an operator
 * operators are all types prefixed by OP_
 */
uint8_t is_operator(token_type type);

/*
 * splits an input string into tokens
 *
 * @iparam input := input string
 * @oparam n_tokens := length of the returned array
 * @returns an array of tokens
 */
token_t* tokenize(const char* input, int32_t* n_tokens);

#endif
