/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <ctype.h>
#include <stdlib.h>

#include "lex.h"

int next_token(const char** start, token_t* t)
{
    int rc = 0;

    char c = **start;
    if (c == '+')
    {
        *t = (token_t) { OP_ADD, BINARY | ASSOC_L | 1, 0 };
        (*start)++;
    }
    else if (c == '-')
    {
        *t = (token_t) { OP_SUB, BINARY | ASSOC_L | 1, 0 };
        (*start)++;
    }
    else if (c == '*')
    {
        *t = (token_t) { OP_MUL, BINARY | ASSOC_L | 2, 0 };
        (*start)++;
    }
    else if (isdigit(c))
    {
        char* num_end;
        int val = strtol(*start, &num_end, 10);
        if (num_end != NULL)
        {
            *t = (token_t) { LITERAL, 0, val };
            *start = num_end;
        }
        else
        {
            rc = -1;
        }
    }
    else
    {
        rc = -1;
    }

    while (isspace(**start))
        (*start)++;

    return rc;
}

static unsigned resize(token_t** tokens, unsigned size)
{
    unsigned new_size = size * 2;
    *tokens = realloc(*tokens, new_size);
    return new_size;
}

token_t* tokenize(const char* input, int* n_tokens)
{
    int size = 16;
    token_t* tokens = malloc(size * sizeof(token_t));

    int rc;
    const char* start = input;
    // find first non-whitespace character
    while (isspace(*start))
        start++;

    *n_tokens = 0;
    while (*start != 0)
    {
        rc = next_token(&start, &tokens[*n_tokens]);
        if (rc < 0)
        {
            // unexpected token found, return position as error condition
            int pos = start - input;
            *n_tokens = pos | (1 << 31);
            return NULL;
        }

        if (++(*n_tokens) == size)
            size = resize(&tokens, size);
    }

    return tokens;
}
