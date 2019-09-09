/*
 * src/eval.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <stdlib.h>

#include <error.h>
#include <eval.h>

int op_add_impl(token_t* op1, token_t* op2, token_t* res)
{
    // FIXME: check operand types
    int32_t out = op1->value + op2->value;
    init_literal(res, out, 0);
    return 0;
}

int op_sub_impl(token_t* op1, token_t* op2, token_t* res)
{
    // FIXME: check operand types
    int32_t out = op1->value - op2->value;
    init_literal(res, out, 0);
    return 0;
}

int op_mul_impl(token_t* op1, token_t* op2, token_t* res)
{
    // FIXME: check operand types
    int32_t out = op1->value * op2->value;
    init_literal(res, out, 0);
    return 0;
}

int op_pos_impl(token_t* op, token_t* res)
{
    // FIXME: check operand type
    init_literal(res, op->value, 0);
    return 0;
}

int op_neg_impl(token_t* op, token_t* res)
{
    // FIXME: check operand type
    int32_t out = op->value * (-1);
    init_literal(res, out, 0);
    return 0;
}

token_t* shunting_yard(token_t* tokens, int n_tokens, int* n_rpn)
{
    token_t* op_stack  = malloc(n_tokens * sizeof(token_t));
    token_t* out_stack  = malloc(n_tokens * sizeof(token_t));

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
        else if (IS_OPERATOR(tokens[n].type))
        {
            // pop from operator stack - while top has greater precedence or
            // equal precedence & left-associative, and not left parentheses
            // - onto output stack
            while (n_op && (
                PREC_GT(op_stack[n_op - 1], tokens[n]) || (
                    PREC_EQ(op_stack[n_op - 1], tokens[n])
                    && ASSOC(op_stack[n_op - 1]) == ASSOC_L))
                && (op_stack[n_op - 1].type != L_PAREN))
            {
                STACK_PUSH(out_stack, n_out, STACK_POP(op_stack, n_op));
            }
            // push token to operator stack
            STACK_PUSH(op_stack, n_op, tokens[n]);
        }
        // if token is a left parentheses, push to operator stack
        else if (tokens[n].type == L_PAREN)
        {
            STACK_PUSH(op_stack, n_op, tokens[n]);
        }
        // if token is a right parentheses
        else if (tokens[n].type == R_PAREN)
        {
            // pop from the operator stack - while top is not a left
            // parentheses - onto output stack
            while (n_op && op_stack[n_op - 1].type != L_PAREN)
            {
                STACK_PUSH(out_stack, n_out, STACK_POP(op_stack, n_op));
            }
            // if there is a left parentheses at the top of the stack, discard
            if (n_op && op_stack[n_op - 1].type == L_PAREN)
            {
                n_op--;
            }
            // otherwise there are mismatched parentheses
            else
            {
                n_out = E_UNMATCHED_PAREN | tokens[n].offset;
                n = n_tokens;
                n_op = 0;
            }
        }
        n++;
    }
    // if operator stack is non-empty, pop everything to output queue
    while (n_op)
    {
        token_t op = STACK_POP(op_stack, n_op);
        // if popped operator is a parentheses, mismatched parentheses
        if (op.type == L_PAREN || op.type == R_PAREN)
        {
            n_out = E_UNMATCHED_PAREN | op.offset;
            n_op = 0;
        }
        else
        {
            STACK_PUSH(out_stack, n_out, op);
        }
    }

    *n_rpn = n_out;
    // free out_stack if an error occurred
    if (n_out < 0)
    {
        free(out_stack);
        out_stack = NULL;
    }
    free(op_stack);

    return out_stack;
}

int evaluate_rpn(token_t* rpn, int n_rpn, token_t* res)
{
    int rc = 0;
    token_t* stack = malloc(n_rpn * sizeof(token_t));
    int n_stack = 0;

    for (int n = 0; n < n_rpn; n++)
    {
        if (IS_OPERATOR(rpn[n].type))
        {
            // unary operator
            if (ARITY(rpn[n]) == 1)
            {
                if (n_stack < 1)
                {
                    // FIXME
                }
                token_t op = STACK_POP(stack, n_stack);
                // evaluate result
                rc = OP_UN(rpn[n])(&op, &stack[n_stack++]);
                if (rc < 0)
                {
                    // error evaluating expression, terminate
                    n = n_rpn;
                }
            }
            // binary operator
            else if (ARITY(rpn[n]) == 2)
            {
                if (n_stack < 2)
                {
                    // FIXME
                }
                token_t op2 = STACK_POP(stack, n_stack);
                token_t op1 = STACK_POP(stack, n_stack);
                // evaluate result
                rc = OP_BIN(rpn[n])(&op1, &op2, &stack[n_stack++]);
                if (rc < 0)
                {
                    // error evaluating expression, terminate
                    n = n_rpn;
                }
            }
            // otherwise, invalid operator encountered
            else
            {
                // FIXME
            }

        }
        else
        {
            // push operand token onto the stack
            STACK_PUSH(stack, n_stack, rpn[n]);
        }
    }

    if (rc < 0)
    {
        res = NULL;
    }
    else
    {
        *res = STACK_POP(stack, n_stack);
    }

    free(stack);
    return 0;
}
