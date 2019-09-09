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

int eval_expr(token_t* expr, int32_t n_tokens, token_t* result)
{
    int rc = 0;
    // convert infix expression to Reverse Polish (postfix) notation
    int32_t n_rpn;
    token_t* rpn = shunting_yard(expr, n_tokens, &n_rpn);
    if (n_rpn < 0)
    {
        rc = n_rpn;
    }
    else
    {
        // evaluate postfix expression
        rc = evaluate_rpn(rpn, n_rpn, result);
        free(rpn);
    }

    return rc;
}

void repl()
{
    const char* prompt = "\033[1;33m>\033[1;32m>\033[1;34m>\033[0m ";
    char input[MAX_INPUT_LEN];
    // main REPL loop
    while (1)
    {
        printf("\n%s", prompt);
        if (!fgets(input, MAX_INPUT_LEN, stdin))
        {
            break;
        }
        // lex input string into tokens
        int32_t n_tokens;
        token_t* tokens = tokenize(input, &n_tokens);
        if (n_tokens < 0)
        {
            print_err(n_tokens, tokens);
        }

        // evaluate input
        token_t result;
        int rc = eval_expr(tokens, n_tokens, &result);
        if (rc < 0)
        {
            print_err(rc, tokens);
        }
        else
        {
            printf("%d\n", result.value);
        }
        free(tokens);
    }
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        repl();
    }
    else
    {
        // lex input string into tokens
        int32_t n_tokens;
        token_t* tokens = tokenize(argv[1], &n_tokens);
        if (n_tokens < 0)
        {
            print_err(n_tokens, tokens);
            return EXIT_FAILURE;
        }

        // evaluate input
        token_t result;
        int rc = eval_expr(tokens, n_tokens, &result);
        if (rc < 0)
        {
            print_err(rc, tokens);
            return EXIT_FAILURE;
        }

        printf("%d\n", result.value);
        free(tokens);
    }

    return EXIT_SUCCESS;
}
