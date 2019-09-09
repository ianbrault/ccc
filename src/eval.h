/*
 * src/eval.h
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef EVAL_H
#define EVAL_H

#include <lex.h>

#define STACK_PUSH(stack, count, item) stack[count++] = item
#define STACK_POP(stack, count) stack[--count]

int op_add_impl(token_t* op1, token_t* op2, token_t* res);
int op_sub_impl(token_t* op1, token_t* op2, token_t* res);
int op_mul_impl(token_t* op1, token_t* op2, token_t* res);

int op_pos_impl(token_t* op, token_t* res);
int op_neg_impl(token_t* op, token_t* res);

// applies a binary operator to its operands
static int (*ops_binary[N_BINARY_OPS])(token_t*, token_t*, token_t*) = {
    [OP_ADD - OP_ADD] = &op_add_impl,
    [OP_SUB - OP_ADD] = &op_sub_impl,
    [OP_MUL - OP_ADD] = &op_mul_impl,
};

// applies a unary operator to its operand
static int (*ops_unary[N_UNARY_OPS])(token_t*, token_t*) = {
    [OP_POS - OP_POS] = &op_pos_impl,
    [OP_NEG - OP_POS] = &op_neg_impl,
};

#define OP_BIN(token) (ops_binary[token.type - OP_ADD])
#define OP_UN(token) (ops_unary[token.type - OP_POS])

/*
 * converts an infix array of tokens into Reverse Polish (postfix) notation
 * using the shunting-yard algorithm
 *
 * @iparam tokens := array of tokens in infix notation
 * @iparam n_tokens := length of tokens array
 * @oparam n_rpn := length of the returned array, shunting-yard removes the
 *                  parentheses so this will likely be different from n_tokens
 * @returns an array of tokens in Reverse Polish notation
 */
token_t* shunting_yard(token_t* tokens, int n_tokens, int* n_rpn);

/*
 * evaluate an expression in Reverse Polish (postfix) notation
 *
 * @iparam rpn := array of tokens in postfix notation
 * @iparam n_rpn := length of RPN array
 * @oparam res := expression result; for now, expect an integer literal; in the
 *                future, this can be used for things like variable assignment
 * @returns 0 on success, otherwise an error code
 */
int evaluate_rpn(token_t* rpn, int n_rpn, token_t* res);

#endif
