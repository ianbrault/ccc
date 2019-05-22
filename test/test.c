/*
 * test/test.c
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <limits.h>

#include "cgreen.h"
#include "lex.h"

Ensure(test_tokenize_valid)
{
    int n_tokens;
    const char* input = "  1+  23 * -456";
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t != NULL);
    assert_that(n_tokens == 6);
    assert_that(t[0].type == LITERAL);
    assert_that(t[0].value == 1);
    assert_that(t[1].type == OP_ADD);
    assert_that(t[1].flags == (BINARY | ASSOC_L | 1));
    assert_that(t[2].type == LITERAL);
    assert_that(t[2].value == 23);
    assert_that(t[3].type == OP_MUL);
    assert_that(t[3].flags == (BINARY | ASSOC_L | 2));
    assert_that(t[4].type == OP_SUB);
    assert_that(t[4].flags == (BINARY | ASSOC_L | 1));
    assert_that(t[5].type == LITERAL);
    assert_that(t[5].value == 456);

    free(t);
}

Ensure(test_tokenize_valid_invalid_syntax)
{
    int n_tokens;
    const char* input = " * * 123 0  ";
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t != NULL);
    assert_that(n_tokens == 4);
    assert_that(t[0].type == OP_MUL);
    assert_that(t[1].type == OP_MUL);
    assert_that(t[2].type == LITERAL);
    assert_that(t[3].type == LITERAL);

    free(t);
}

Ensure(test_tokenize_invalid)
{
    int n_tokens;
    const char* input = "  32 * abc";
    token_t* t = tokenize(input, &n_tokens);

    assert_that(t == NULL);
    assert_that(n_tokens == (7 | INT_MIN));
}

int main(int argc, char **argv)
{
    TestSuite *suite = create_test_suite();

    add_test(suite, test_tokenize_valid);
    add_test(suite, test_tokenize_valid_invalid_syntax);
    add_test(suite, test_tokenize_invalid);

    return run_test_suite(suite, create_text_reporter());
}
