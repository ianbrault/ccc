/*
 * src/eval.h
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef EVAL_H
#define EVAL_H

#include <lex.h>

/*
 * converts an infix array of tokens into Reverse Polish notation using the shunting-yard algorithm
 * @iparam tokens := array of tokens in infix notation
 * @iparam n_toksn := length of tokens array
 * @returns array of tokens in Reverse Polish notation
 */
token_t* shunting_yard(token_t* tokens, int n_tokens);

#endif