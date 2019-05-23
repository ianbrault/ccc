/*
 * src/main.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lex.h"

void eprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "\033[0;31merror:\033[0m ");
    vfprintf(stderr, format, args);

    va_end(args);
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        eprintf("REPL is not yet supported\n");
        return EXIT_FAILURE;
    }

    const char* input = argv[1];

    int n_tokens;
    token_t* tokens = tokenize(input, &n_tokens);
    if (tokens == NULL)
    {
        int pos = n_tokens & (~INT_MIN);
        eprintf("unexpected token at position %d: %c\n", pos, input[pos]);
        return EXIT_FAILURE;
    }

    printf("lexed input, found %d tokens\n", n_tokens);

    free(tokens);
    return EXIT_SUCCESS;
}
