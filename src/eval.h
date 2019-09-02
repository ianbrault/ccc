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

#endif
