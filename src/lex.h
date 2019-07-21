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
    OP_ADD,
    OP_SUB,
    OP_MUL,
    LITERAL,
} token_type;

#define UNARY  0
#define BINARY (1 << 7)

#define ASSOC_L 0
#define ASSOC_R (1 << 6)

#define PRECEDENCE_OP_ADD_UNARY  1
#define PRECEDENCE_OP_SUB_UNARY  1
#define PRECEDENCE_OP_MUL        2
#define PRECEDENCE_OP_ADD_BINARY 3
#define PRECEDENCE_OP_SUB_BINARY 3

typedef struct {
    token_type type;
    // flags are used for operator tokens, will be set to 0 for literals
    // bit 7: binary vs. unary
    // bit 6: associativity (left vs. right)
    // bits 5-0: precedence
    uint8_t flags;
    // value of literals
    int32_t value;
    // offset from start of input string, used for error messages
    int32_t offset;
} token_t;

/*
 * splits an input string into tokens
 * @iparam input := input string
 * @oparam tokens := array of tokens
 * @returns the length of the token array
 */
int tokenize(const char* input, token_t** tokens);

#endif
