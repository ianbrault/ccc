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

void print_err(int errno)
{
    // NOTE: E_INVALID_TOKEN must be checked first
    // lower 30 bits contain offset info which could falsely flag other errors
    if (errno & e_invalid_token_flag)
    {
        int32_t pos = errno & ~(0x3 << 30);
        eprintf("invalid token at position %d\n", pos);
    }
    else if (errno & e_max_tokens_flag)
    {
        eprintf("input string is too long; a maximum of 256 tokens is accepted\n");
    }
}