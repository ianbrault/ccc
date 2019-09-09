/*
 * src/error.c
 * utilities to print error messages
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <error.h>

void eprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "\033[0;31;1merror:\033[0m ");
    vfprintf(stderr, format, args);

    va_end(args);
}

static char operator_to_char(token_t token)
{
    char ch = 0;
    switch (token.type)
    {
    case L_PAREN:
        ch = '(';
        break;
    case R_PAREN:
        ch = ')';
        break;
    case OP_ADD:
    case OP_POS:
        ch = '+';
        break;
    case OP_SUB:
    case OP_NEG:
        ch = '-';
        break;
    case OP_MUL:
        ch = '*';
        break;
    default:
        break;
    }

    return ch;
}

static int32_t e_max_tokens_flag       = 0x1 << 30;
static int32_t e_max_input_flag        = 0x1 << 29;
static int32_t e_invalid_token_flag    = 0x1 << 28;
static int32_t e_unmatched_paren_flag  = 0x1 << 27;
static int32_t e_op_missing_expr_flag  = 0x1 << 26;
static int32_t e_invalid_lit_expr_flag = 0x1 << 25;

// given a token offset, return the index in the token array
static int32_t get_index_from_offset(token_t* tokens, int32_t offset)
{
    // NOTE: this has the potential to raise a null-pointer exception if called
    // with an offset that does not exist, but this is a risk taken for API
    // simplicity; possibly rework if issues arise
    int index = -1;
    for (int i = 0; i < MAX_TOKENS; i++)
    {
        if (tokens[i].offset == offset)
        {
            index = i;
            break;
        }
    }
    return index;
}

void print_err(int errno, token_t* tokens)
{
    if (errno & e_max_tokens_flag)
    {
        eprintf("maximum number of tokens (%d) exceeeded\n", MAX_TOKENS);
    }
    else if (errno & e_max_input_flag)
    {
        eprintf("maximum input length (%d) exceeded\n", MAX_INPUT_LEN);
    }
    else if (errno & e_invalid_token_flag)
    {
        // get token offset
        int32_t pos = errno & 0x3ff;
        eprintf("%d: invalid token\n", pos);
    }
    else if (errno & e_unmatched_paren_flag)
    {
        // get token offset and index into tokens array
        int32_t pos = errno & 0x3ff;
        int32_t index = get_index_from_offset(tokens, pos);
        char paren = tokens[index].type == L_PAREN ? '(' : ')';
        eprintf("%d: unmatched \"%c\"\n", pos, paren);
    }
    else if (errno & e_op_missing_expr_flag)
    {
        // get token offset and index into tokens array
        int32_t pos = errno & 0x3ff;
        int32_t index = get_index_from_offset(tokens, pos);
        char op_str = operator_to_char(tokens[index]);
        // get side information
        int32_t side = errno & (0x1 << 10);
        const char* side_str = side ? "right" : "left";
        eprintf(
            "%d: operator \"%c\" missing %s-hand expression\n",
            pos, op_str, side_str);
    }
    else if (errno & e_invalid_lit_expr_flag)
    {
        // get token offset and index into tokens array
        int32_t pos = errno & 0x3ff;
        int32_t index = get_index_from_offset(tokens, pos);
        char lit_str[32];
        snprintf(lit_str, 32, "%d", tokens[index].value);
        eprintf(
            "%d: %s must be followed by an operator or end of expression\n",
            pos, lit_str);
    }
}