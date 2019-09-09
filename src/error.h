/*
 * src/error.h
 * utilities to print error messages
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef ERROR_H
#define ERROR_H

#include <lex.h>

/* ERROR NUMBERS
 * define new error codes below
 * the format currently allows for 11 error codes
 *
 * +----+-----------------+----------------------+
 * | 31 | 30    ...    20 | 19       ...      00 |
 * +----+-----------------+----------------------+
 * |  1 |       flag      |  further error info  |
 * +----+-----------------+----------------------+
 */

// LEX_H

// if MAX_TOKENS is exceeded
#define E_MAX_TOKENS       (INT32_MIN | (0x1 << 30))
// if MAX_INPUT is exceeded
#define E_MAX_INPUT        (INT32_MIN | (0x1 << 29))
// invalid token encountered
// bits 0-9 contain the token offset
#define E_INVALID_TOKEN    (INT32_MIN | (0x1 << 28))

// EVAL_H

// unmatched parenthesis
// bits 0-9 contain the token offset
#define E_UNMATCHED_PAREN  (INT32_MIN | (0x1 << 27))
// operator missing either left-side or right-side expression
// bits 0-9 contain the operator token offset
// bit 10 indicates the side (0 for left, 1 for right)
#define E_OP_MISSING_EXPR  (INT32_MIN | (0x1 << 26))
// literal was not followed by an operator or expression termination
// bits 0-9 contain the token offset
#define E_INVALID_LIT_EXPR (INT32_MIN | (0x1 << 25))

/*
 * print an error message with a red "error:" prepended
 * variadic arguments have the same semantics as the printf family
 */
void eprintf(const char* format, ...);

/*
 * print an error message corresponding to an error code
 *
 * @iparam errno := error code
 * @iparam tokens := token array lexed from input string, used for informative
 *                   error messages
 */
void print_err(int errno, token_t* tokens);

#endif
