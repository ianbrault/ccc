/*
 * src/eval.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <stdlib.h>

#include <error.h>
#include <eval.h>

void op_add_impl(token_t* op1, token_t* op2, token_t* res)
{
    int32_t out = op1->value + op2->value;
    init_literal(res, out, 0);
}

void op_sub_impl(token_t* op1, token_t* op2, token_t* res)
{
    int32_t out = op1->value - op2->value;
    init_literal(res, out, 0);
}

void op_mul_impl(token_t* op1, token_t* op2, token_t* res)
{
    int32_t out = op1->value * op2->value;
    init_literal(res, out, 0);
}

void op_pos_impl(token_t* op, token_t* res)
{
    init_literal(res, op->value, 0);
}

void op_neg_impl(token_t* op, token_t* res)
{
    int32_t out = op->value * (-1);
    init_literal(res, out, 0);
}

token_t* shunting_yard(token_t* tokens, int n_tokens, int* n_rpn)
{
    token_t* op_stack  = malloc(n_tokens * sizeof(token_t));
    token_t* out_stack  = malloc(n_tokens * sizeof(token_t));
    int n = 0, n_out = 0, n_op = 0;
    int literal_was_prev = 0;

    // an operator at the start must be unary and right-associative
    if (IS_OPERATOR(tokens[0]))
    {
        if (ARITY(tokens[0]) != 1 || ASSOC(tokens[0]) != ASSOC_R)
        {
            n_out = E_OP_MISSING_EXPR | tokens[0].offset;
            n = n_tokens;
        }
    }
    // an operator at the end must be unary and left-associative
    int end = n_tokens - 1;
    if (!n && IS_OPERATOR(tokens[end]))
    {
        if (ARITY(tokens[end]) != 1 || ASSOC(tokens[end]) != ASSOC_L)
        {
            n_out = E_OP_MISSING_EXPR;
            n_out |= (0x1 << 10) | tokens[end].offset;
            n = n_tokens;
        }
    }

    // while there are still tokens to be read
    while (n < n_tokens)
    {
        // if token is a number, push to output stack
        if (IS_LITERAL(tokens[n]))
        {
            // literal cannot follow another literal
            if (literal_was_prev)
            {
                n_out = E_INVALID_LIT_EXPR | tokens[n - 1].offset;
                n = n_tokens;
            }
            else
            {
                literal_was_prev = 1;
                STACK_PUSH(out_stack, n_out, tokens[n]);
            }
        }
        // if token is an operator
        else if (IS_OPERATOR(tokens[n]))
        {
            literal_was_prev = 0;
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
            // left parenthesis cannot follow another literal
            if (literal_was_prev)
            {
                n_out = E_INVALID_LIT_EXPR | tokens[n - 1].offset;
                n = n_tokens;
            }
            else
            {
                literal_was_prev = 0;
                STACK_PUSH(op_stack, n_op, tokens[n]);
            }
        }
        // if token is a right parentheses
        else if (tokens[n].type == R_PAREN)
        {
            literal_was_prev = 0;
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
        if (IS_OPERATOR(rpn[n]))
        {
            // unary operator
            if (ARITY(rpn[n]) == 1)
            {
                // conditions in lex.c:add_unary_pos_neg_ops are strong enough
                // to ensure that there is an operand on the stack
                token_t op = STACK_POP(stack, n_stack);
                // evaluate result
                OP_UN(rpn[n])(&op, &stack[n_stack++]);
            }
            // binary operator
            else if (ARITY(rpn[n]) == 2)
            {
                // not enough operands on the stack
                if (n_stack < 2)
                {
                    rc = E_OP_MISSING_EXPR | (0x1 << 10) | rpn[n].offset;
                    n = n_rpn;
                }
                else
                {
                    token_t op2 = STACK_POP(stack, n_stack);
                    token_t op1 = STACK_POP(stack, n_stack);
                    // evaluate result
                    OP_BIN(rpn[n])(&op1, &op2, &stack[n_stack++]);
                }
            }
        }
        else
        {
            // push operand token onto the stack
            STACK_PUSH(stack, n_stack, rpn[n]);
        }
    }

    if (!rc)
    {
        *res = STACK_POP(stack, n_stack);
    }

    free(stack);
    return rc;
}
