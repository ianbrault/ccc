/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef LEX_H
#define LEX_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    LITERAL,
} token_type;

#define UNARY  0
#define BINARY (1 << 7)

#define ASSOC_L 0
#define ASSOC_R (1 << 6)

typedef struct {
    token_type type;
    // flags are used for operator tokens, will be set to 0 for literals
    // bit 7: binary vs. unary
    // bit 6: associativity (left vs. right)
    // bits 5-0: precedence
    uint8_t flags;
    // value of literals
    int32_t value;
} token_t;

/*
 * get the next token struct from a string
 * @iparam start := start position of token
 * @oparam t := token struct to be created
 * @returns 0 on success, -1 if an invalid token was encountered
 */
int next_token(const char** start, token_t* t);

/*
 * splits an input string into tokens
 * @iparam input := input string
 * @oparam n_tokens := the number of tokens
 * @returns an array of tokens
 */
token_t* tokenize(const char* input, int* n_tokens);

#endif
