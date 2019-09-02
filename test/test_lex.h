/*
 * test/test_lex.h
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <cgreen/cgreen.h>

#include <error.h>
#include <lex.h>

Ensure(test_tokenize_valid)
{
    const char* input = "  +1+  23 * -456";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t != NULL);
    assert_that(n_tokens == 7);

    assert_that(t[0].type == OP_ADD);
    assert_that(t[0].offset == 2);

    assert_that(t[1].type == LITERAL);
    assert_that(t[1].value == 1);
    assert_that(t[1].offset == 3);

    assert_that(t[2].type == OP_ADD);
    assert_that(t[2].offset == 4);

    assert_that(t[3].type == LITERAL);
    assert_that(t[3].value == 23);
    assert_that(t[3].offset == 7);

    assert_that(t[4].type == OP_MUL);
    assert_that(t[4].offset == 10);

    assert_that(t[5].type == OP_SUB);
    assert_that(t[5].offset == 12);

    assert_that(t[6].type == LITERAL);
    assert_that(t[6].value == 456);
    assert_that(t[6].offset == 13);

    free(t);
}

Ensure(test_tokenize_valid_invalid_syntax)
{
    const char* input = " * * 123 0  ";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t != NULL);
    assert_that(n_tokens == 4);

    assert_that(t[0].type == OP_MUL);
    assert_that(t[1].type == OP_MUL);
    assert_that(t[2].type == LITERAL);
    assert_that(t[2].value == 123);
    assert_that(t[3].type == LITERAL);
    assert_that(t[4].value == 0);

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