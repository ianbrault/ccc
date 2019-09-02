/*
 * src/eval.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <stdlib.h>

#include <eval.h>

token_t* shunting_yard(token_t* tokens, int n_tokens)
{
    token_t* out_stack = malloc(n_tokens * sizeof(token_t));
    token_t* op_stack  = malloc(n_tokens * sizeof(token_t));

    int n = 0, n_out = 0, n_op = 0;
    // while there are still tokens to be read
    while (n < n_tokens)
    {
        // if token is a number, push to output stack
        if (tokens[n].type == LITERAL)
        {
            STACK_PUSH(out_stack, n_out, tokens[n]);
        }
        // if token is an operator
        else if (is_operator(tokens[n].type))
        {
            // while there is an operator on top of the operator stack with
            // greater precedence or with equal precedence & left-associative,
            // pop from the operator stack to the output stack
            while (n_op && (
                PREC(op_stack[n_op - 1]) < PREC(tokens[n])
                || (
                    PREC(op_stack[n_op - 1]) == PREC(tokens[n])
                    && ASSOC(op_stack[n_op - 1]) == ASSOC_L)))
            {
                STACK_PUSH(out_stack, n_out, STACK_POP(op_stack, n_op));
            }
            // push token to operator stack
            STACK_PUSH(op_stack, n_op, tokens[n]);
        }
        n++;
    }
    // if operator stack is non-empty, pop everything to output queue
    while (n_op)
    {
        STACK_PUSH(out_stack, n_out, STACK_POP(op_stack, n_op));
    }

    free(op_stack);
    return out_stack;
}
