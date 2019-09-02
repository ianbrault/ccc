/*
 * test/utils.c
 * common utilities used across the unit test suite
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <utils.h>

uint8_t token_is_op(token_t token, token_type op_type)
{
    return token.type == op_type;
}

uint8_t token_is_literal(token_t token, int32_t value)
{
    return token.type == LITERAL && token.value == value;
}