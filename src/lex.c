/*
 * src/lex.h
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <ctype.h>
#include <stdlib.h>

#include "lex.h"

#define N_OPERATORS 2
static const char operators[N_OPERATORS] = { '+', '-' };

int is_operator(char c)
{
    for (int i = 0; i < N_OPERATORS; i++)
    {
        if (c == operators[i])
            return 1;
    }

    return 0;
}

unsigned resize(token* tokens, unsigned size)
{
    unsigned new_size = size * 2;
    tokens = realloc(tokens, new_size);
    return new_size;
}

int tokenize(const char* input, token* tokens)
{
    if (tokens != NULL)
        free(tokens);

    // reallocation policy will be to double the size when it is exceeded
    // more research is needed to determine if this is an appropriate starting value
    // the idea is that most calculations will be relatively short
    unsigned size = 16;
    tokens = malloc(size * sizeof(token));

    const char* start = input;
    const char* end;
    unsigned n_tokens = 0;

    while (*start != 0)
    {
        while (isspace(*start))
            start++;

        if (is_operator(*start))
        {
            tokens[n_tokens] = (token) { .repr = start, .len = 1, .type = OPERATOR };
            if (++n_tokens == size)
                size = resize(tokens, size);

            start++;
        }
        else if (isdigit(*start))
        {
            // find end of number
            end = start + 1;
            while (isdigit(*end))
                end++;

            tokens[n_tokens] = (token) { .repr = start, .len = (end - start), .type = LITERAL };
            if (++n_tokens == size)
                size = resize(tokens, size);

            start = end;
        }
        else
        {
            // unexpected token found, return position as error condition
            int pos = start - input;
            return pos | (1 << 31);
        }
    }

    return n_tokens;
}
