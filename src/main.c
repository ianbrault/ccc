/*
 * src/main.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <error.h>
#include <eval.h>
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
    if (n_tokens < 0)
    {
        print_err(n_tokens, tokens);
        return EXIT_FAILURE;
    }

    // convert infix expression to Reverse Polish (postfix) notation
    int32_t n_rpn;
    token_t* rpn = shunting_yard(tokens, n_tokens, &n_rpn);
    if (n_rpn < 0)
    {
        print_err(n_rpn, tokens);
        free(tokens);
        return EXIT_FAILURE;
    }

    // evaluate postfix expression
    token_t res;
    int rc = evaluate_rpn(rpn, n_rpn, &res);
    if (rc < 0)
    {
        print_err(rc, tokens);
        free(rpn);
        free(tokens);
        return EXIT_FAILURE;
    }
    // if evaluation was successful, print result
    printf("%d\n", res.value);

    free(tokens);
    free(rpn);
    return EXIT_SUCCESS;
}
