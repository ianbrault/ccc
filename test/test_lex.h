/*
 * test/test_lex.h
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <cgreen/cgreen.h>

#include <error.h>
#include <lex.h>
#include <utils.h>

Ensure(test_tokenize_valid)
{
    const char* input = "  +1+  23 * (-456 +0 )";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t != NULL);
    assert_that(n_tokens == 11);

    assert_that(token_is_op(t[0], OP_ADD));
    assert_that(t[0].offset == 2);

    assert_that(token_is_literal(t[1], 1));
    assert_that(t[1].offset == 3);

    assert_that(token_is_op(t[2], OP_ADD));
    assert_that(t[2].offset == 4);

    assert_that(token_is_literal(t[3], 23));
    assert_that(t[3].offset == 7);

    assert_that(token_is_op(t[4], OP_MUL));
    assert_that(t[4].offset == 10);

    assert_that(token_is_op(t[5], L_PAREN));
    assert_that(t[5].offset == 12);

    assert_that(token_is_op(t[6], OP_SUB));
    assert_that(t[6].offset == 13);

    assert_that(token_is_literal(t[7], 456));
    assert_that(t[7].offset == 14);

    assert_that(token_is_op(t[8], OP_ADD));
    assert_that(t[8].offset == 18);

    assert_that(token_is_literal(t[9], 0));
    assert_that(t[9].offset == 19);

    assert_that(token_is_op(t[10], R_PAREN));
    assert_that(t[10].offset == 21);

    free(t);
}

Ensure(test_tokenize_valid_invalid_syntax)
{
    const char* input = " * * 123 0  ";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t != NULL);
    assert_that(n_tokens == 4);

    assert_that(token_is_op(t[0], OP_MUL));
    assert_that(token_is_op(t[1], OP_MUL));
    assert_that(token_is_literal(t[2], 123));
    assert_that(token_is_literal(t[3], 0));

    free(t);
}

Ensure(test_tokenize_invalid)
{
    const char* input = "  32 * abc";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t == NULL);
    assert_that(n_tokens == (E_INVALID_TOKEN | 7));
}