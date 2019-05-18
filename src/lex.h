/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef LEX_H
#define LEX_H

#include <stddef.h>

typedef enum {
    LITERAL,
    OPERATOR,
} token_type;

typedef struct {
    // avoids string copying by pointing directly into the input string
    const char* repr;
    int len;
    token_type type;
} token;

/*
 * splits an input string into tokens
 * @iparam input := input string
 * @oparam tokens := array of tokens
 * @returns the number of tokens
 */
int tokenize(const char* input, token* tokens);

#endif
