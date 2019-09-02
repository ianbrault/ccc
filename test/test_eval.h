/*
 * test/test_eval.h
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <cgreen/cgreen.h>

#include <eval.h>
#include <lex.h>

Ensure(test_shunting_yard_basic)
{
    const char* input = "1 + 2 * 3";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 5);

    token_t* rpn = shunting_yard(t, n_tokens);
    assert_that(rpn != NULL);
    // RPN should be: 1 2 3 * +
    assert_that(rpn[0].type == LITERAL && rpn[0].value == 1);
    assert_that(rpn[1].type == LITERAL && rpn[1].value == 2);
    assert_that(rpn[2].type == LITERAL && rpn[2].value == 3);
    assert_that(rpn[3].type == OP_MUL);
    assert_that(rpn[4].type == OP_ADD);
}

Ensure(test_shunting_yard_addition_chain)
{
    const char* input = "0+1+2+3+4";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 9);

    token_t* rpn = shunting_yard(t, n_tokens);
    assert_that(rpn != NULL);
    // RPN should be: 0 1 + 2 + 3 + 4 +
    assert_that(rpn[0].type == LITERAL && rpn[0].value == 0);
    assert_that(rpn[1].type == LITERAL && rpn[1].value == 1);
    assert_that(rpn[2].type == OP_ADD);
    assert_that(rpn[3].type == LITERAL && rpn[3].value == 2);
    assert_that(rpn[4].type == OP_ADD);
    assert_that(rpn[5].type == LITERAL && rpn[5].value == 3);
    assert_that(rpn[6].type == OP_ADD);
    assert_that(rpn[7].type == LITERAL && rpn[7].value == 4);
    assert_that(rpn[8].type == OP_ADD);
}

Ensure(test_shunting_yard_only_numbers)
{
    // numbers should appear in output stack in the same order
    const char* input = "1 2 3 4 5 6";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 6);

    token_t* rpn = shunting_yard(t, n_tokens);
    assert_that(rpn != NULL);
    for (int i = 0; i < n_tokens; i++)
    {
        assert_that(t[i].type == rpn[i].type);
        assert_that(t[i].value == rpn[i].value);
        assert_that(t[i].offset == rpn[i].offset);
    }

    free(t);
    free(rpn);
}
