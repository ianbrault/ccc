/*
 * src/error.h
 * utilities to print error messages
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef ERROR_H
#define ERROR_H

/***         ERROR NUMBERS        ***/
/*** define new error codes below ***/

// LEX_H

// invalid token encountered
// lower 16 bits will contain the offset
#define E_INVALID_TOKEN (INT32_MIN | (0x1 << 30))
// if MAX_TOKENS is exceeded
#define E_MAX_TOKENS (INT32_MIN | 0x1)

// EVAL_H

// unmatched parenthesis
// lower 16 bits will contain the offset
#define E_UNMATCHED_PAREN (INT32_MIN | (0x1 << 29))

/*
 * print an error message with a red "error:" prepended
 * variadic arguments have the same semantics as the printf family
 */
void eprintf(const char* format, ...);

/*
 * print an error message corresponding to an error code
 */
void print_err(int errno);

#endif
