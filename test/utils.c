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

uint8_t token_is_int(token_t token, int64_t value)
{
    uint8_t rc = token.type == LITERAL;
    rc &= token.value.type == INT && token.value.value.i == value;
    return rc;
}

uint8_t token_is_float(token_t token, double value)
{
    uint8_t rc = token.type == LITERAL;
    rc &= token.value.type == FLOAT && token.value.value.f == value;
    return rc;
}