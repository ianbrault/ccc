/*
 * src/error.c
 * utilities to print error messages
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <stdarg.h>
#include <stdio.h>

#include <error.h>

void eprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "\033[0;31merror:\033[0m ");
    vfprintf(stderr, format, args);

    va_end(args);
}

static int32_t e_invalid_token_flag = 0x1 << 30;
static int32_t e_max_tokens_flag = 0x1;
static int32_t e_unmatched_paren_flag = 0x1 << 29;

void print_err(int errno)
{
    // NOTE: E_INVALID_TOKEN must be checked first
    // lower 16 bits contain offset info which could falsely flag other errors
    if (errno & e_invalid_token_flag)
    {
        int32_t pos = errno & 0xffff;
        eprintf("invalid token at position %d\n", pos);
    }
    // NOTE: for the same reason as above, check E_UNMATCHED_PAREN next
    else if (errno & e_unmatched_paren_flag)
    {
        int32_t pos = errno & 0xffff;
        eprintf("could not match the parenthesis at position %d\n", pos);
    }
    else if (errno & e_max_tokens_flag)
    {
        eprintf("input string is too long; a maximum of 256 tokens is accepted\n");
    }
}