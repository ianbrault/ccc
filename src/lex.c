/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

#include "lex.h"

int next_token(const char** start, token_t* t)
{
    int rc = 0;

    char c = **start;
    if (c == '+')
    {
        *t = (token_t) { OP_ADD, BINARY | ASSOC_L | PRECEDENCE_OP_ADD_BINARY, 0 };
        (*start)++;
    }
    else if (c == '-')
    {
        *t = (token_t) { OP_SUB, BINARY | ASSOC_L | PRECEDENCE_OP_SUB_BINARY, 0 };
        (*start)++;
    }
    else if (c == '*')
    {
        *t = (token_t) { OP_MUL, BINARY | ASSOC_L | PRECEDENCE_OP_MUL, 0 };
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

static int is_operator(token_type type)
{
    return type == OP_ADD || type == OP_SUB || type == OP_MUL;
}

// determine which binary operators should be unary
static void parse_unary_operators(token_t* tokens, int n_tokens)
{
    for (int i = 0; i < n_tokens; i++)
    {
        if (tokens[i].type == OP_ADD || tokens[i].type == OP_SUB)
        {
            if (i == 0 || (i > 0 && is_operator(tokens[i - 1].type)))
                tokens[i].flags = UNARY | ASSOC_L | PRECEDENCE_OP_ADD_UNARY;
        }
    }
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
            *n_tokens = pos | INT_MIN;
            return NULL;
        }

        if (++(*n_tokens) == size)
            size = resize(&tokens, size);
    }

    parse_unary_operators(tokens, *n_tokens);

    return tokens;
}
