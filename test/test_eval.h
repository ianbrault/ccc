/*
 * test/test_eval.h
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <cgreen/cgreen.h>

#include <error.h>
#include <eval.h>
#include <lex.h>
#include <utils.h>

Ensure(test_shunting_yard_basic)
{
    const char* input = "1 + 2 * 3";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 5);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);
    assert_that(n_rpn == 5);
    // RPN should be: 1 2 3 * +
    assert_that(token_is_literal(rpn[0], 1));
    assert_that(token_is_literal(rpn[1], 2));
    assert_that(token_is_literal(rpn[2], 3));
    assert_that(token_is_op(rpn[3], OP_MUL));
    assert_that(token_is_op(rpn[4], OP_ADD));

    free(t);
}

Ensure(test_shunting_yard_parens)
{
    const char* input = "3 * (4 + 2) - ((1 - 5) * 3)";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 17);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);
    assert_that(n_rpn == 11);
    // RPN should be: 3 4 2 + * 1 5 - 3 * -
    assert_that(token_is_literal(rpn[0], 3));
    assert_that(token_is_literal(rpn[1], 4));
    assert_that(token_is_literal(rpn[2], 2));
    assert_that(token_is_op(rpn[3], OP_ADD));
    assert_that(token_is_op(rpn[4], OP_MUL));
    assert_that(token_is_literal(rpn[5], 1));
    assert_that(token_is_literal(rpn[6], 5));
    assert_that(token_is_op(rpn[7], OP_SUB));
    assert_that(token_is_literal(rpn[8], 3));
    assert_that(token_is_op(rpn[9], OP_MUL));
    assert_that(token_is_op(rpn[10], OP_SUB));

    free(t);
}

Ensure(test_shunting_yard_addition_chain)
{
    const char* input = "0+1+2+3+4";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 9);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);
    assert_that(n_rpn == 9);
    // RPN should be: 0 1 + 2 + 3 + 4 +
    assert_that(token_is_literal(rpn[0], 0));
    assert_that(token_is_literal(rpn[1], 1));
    assert_that(token_is_op(rpn[2], OP_ADD));
    assert_that(token_is_literal(rpn[3], 2));
    assert_that(token_is_op(rpn[4], OP_ADD));
    assert_that(token_is_literal(rpn[5], 3));
    assert_that(token_is_op(rpn[6], OP_ADD));
    assert_that(token_is_literal(rpn[7], 4));
    assert_that(token_is_op(rpn[8], OP_ADD));

    free(t);
}

Ensure(test_shunting_yard_unmatched_lparen)
{
    const char* input = "(1 + 2) - ((3 + 4) + 5";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 14);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn == NULL);
    assert_that(n_rpn == (E_UNMATCHED_PAREN | 10));
}

Ensure(test_shunting_yard_unmatched_rparen)
{
    const char* input = "(1 + 2)) - 5";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);
    assert_that(n_tokens == 8);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn == NULL);
    assert_that(n_rpn == (E_UNMATCHED_PAREN | 7));
}

Ensure(test_eval_add)
{
    const char* input = "1 + 2 + 3 + 4";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);

    token_t res;
    int rc = evaluate_rpn(rpn, n_rpn, &res);
    assert_that(rc == 0);
    assert_that(token_is_literal(res, 10));
}

Ensure(test_eval_sub)
{
    const char* input = "128 - 64 - 32 - 0";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);

    token_t res;
    int rc = evaluate_rpn(rpn, n_rpn, &res);
    assert_that(rc == 0);
    assert_that(token_is_literal(res, 32));
}

Ensure(test_eval_mul)
{
    const char* input = "1 * 2 * 4 * 8";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);

    token_t res;
    int rc = evaluate_rpn(rpn, n_rpn, &res);
    assert_that(rc == 0);
    assert_that(token_is_literal(res, 64));
}

Ensure(test_eval_negation)
{
    const char* input = "10 - (-2) - +2 - (-(-10))";
    int32_t n_tokens;
    token_t* t = tokenize(input, &n_tokens);
    assert_that(t != NULL);

    int32_t n_rpn;
    token_t* rpn = shunting_yard(t, n_tokens, &n_rpn);
    assert_that(rpn != NULL);

    token_t res;
    int rc = evaluate_rpn(rpn, n_rpn, &res);
    assert_that(rc == 0);
    assert_that(token_is_literal(res, 0));
}
