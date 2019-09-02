/*
 * test/utils.h
 * common utilities used across the unit test suite
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <lex.h>

uint8_t token_is_op(token_t token, token_type op_type);
uint8_t token_is_literal(token_t token, int32_t value);

#endif