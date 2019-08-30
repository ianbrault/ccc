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
    OP_ADD,
    OP_SUB,
    OP_MUL,
    N_TOKEN_TYPES
} token_type;

// precedence of operators, taken from "C Operator Precedence"
// https://en.cppreference.com/w/c/language/operator_precedence
static uint8_t prec[N_TOKEN_TYPES] = {
    [INVALID] = 0,
    [LITERAL] = 0,
    [OP_ADD]  = 4,
    [OP_SUB]  = 4,
    [OP_MUL]  = 3,
};

#define ASSOC_L 1
#define ASSOC_R 2

// associativity of operators
static uint8_t assoc[N_TOKEN_TYPES] = {
    [INVALID] = 0,
    [LITERAL] = 0,
    [OP_ADD]  = ASSOC_L,
    [OP_SUB]  = ASSOC_L,
    [OP_MUL]  = ASSOC_L,
};

typedef struct {
    token_type type;
    int32_t value;   // only used for literals
    int32_t offset;  // offset from start of input string, used for errors
} token_t;

// FIXME: temporary max number of tokens
// implement to use a resizing scheme later on
#define MAX_TOKENS (UINT8_MAX)

// invalid token encountered
// lower 30 bits will contain the offset
#define E_INVALID_TOKEN (INT32_MIN | (0x1 << 30))
// if MAX_TOKENS is exceeded
#define E_MAX_TOKENS (INT32_MIN | 0x1)

/*
 * splits an input string into tokens
 * @iparam input := input string
 * @oparam n_tokens := length of the returned array
 * @returns an array of tokens
 */
token_t* tokenize(const char* input, int32_t* n_tokens);

#endif
