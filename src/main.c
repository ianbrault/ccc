/*
 * src/main.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <error.h>
#include <lex.h>

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        eprintf("REPL is not yet supported\n");
        return EXIT_FAILURE;
    }

    const char* input = argv[1];

    // lex input string into tokens
    int32_t n_tokens;
    token_t* tokens = tokenize(input, &n_tokens);
    if (!tokens)
    {
        print_err(n_tokens);
        return EXIT_FAILURE;
    }

    free(tokens);
    return EXIT_SUCCESS;
}
