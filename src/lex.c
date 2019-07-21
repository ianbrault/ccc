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

static const char* next_token_numeric(const char* start, token_t* t)
{
    char* num_end;

    int val = strtol(start, &num_end, 10);
    if (num_end != NULL)
    {
        *t = (token_t) { LITERAL, 0, val, 0 };
    }

    return num_end;
}

static const char* next_token(const char* start, token_t* t)
{
    const char* end;

    char c = *start;
    if (isdigit(c))
    {
        end = next_token_numeric(start, t);
    }
    else if (c == '+')
    {
        *t = (token_t) { OP_ADD, BINARY | ASSOC_L | PRECEDENCE_OP_ADD_BINARY, 0, 0 };
        end = start + 1;
    }
    else if (c == '-')
    {
        *t = (token_t) { OP_SUB, BINARY | ASSOC_L | PRECEDENCE_OP_SUB_BINARY, 0, 0 };
        end = start + 1;
    }
    else if (c == '*')
    {
        *t = (token_t) { OP_MUL, BINARY | ASSOC_L | PRECEDENCE_OP_MUL, 0, 0 };
        end = start + 1;
    }
    else
    {
        end = NULL;
    }

    while (end != NULL && isspace(*end))
    {
        end++;
    }

    return end;
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
            {
                tokens[i].flags = UNARY | ASSOC_L | PRECEDENCE_OP_ADD_UNARY;
            }
        }
    }
}

static unsigned resize(token_t** tokens, unsigned size)
{
    unsigned new_size = size * 2;
    *tokens = realloc(*tokens, new_size);
    return new_size;
}

int tokenize(const char* input, token_t** tokens)
{
    int n_tokens = 0;

    int size = 16;
    *tokens = malloc(size * sizeof(token_t));

    const char* start = input;
    // find first non-whitespace character
    while (isspace(*start))
    {
        start++;
    }

    while (*start != 0)
    {
        int offset = start - input;
        start = next_token(start, &(*tokens)[n_tokens]);
        // next_token error
        if (start == NULL)
        {
            // unexpected token found, return position as error condition
            n_tokens = offset | INT_MIN;
            free(*tokens);
            return n_tokens;
        }

        (*tokens)[n_tokens].offset = offset;
        if (++n_tokens == size)
        {
            size = resize(tokens, size);
        }
    }

    parse_unary_operators(*tokens, n_tokens);

    return n_tokens;
}
