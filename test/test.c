/*
 * test/test.c
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <cgreen/cgreen.h>

#include <test_eval.h>
#include <test_lex.h>

int main(int argc, char **argv)
{
    TestSuite *suite = create_test_suite();

    // test_lex.h
    add_test(suite, test_tokenize_valid);
    add_test(suite, test_tokenize_valid_invalid_syntax);
    add_test(suite, test_tokenize_invalid);

    // test_eval.h
    add_test(suite, test_shunting_yard_basic);
    add_test(suite, test_shunting_yard_parens);
    add_test(suite, test_shunting_yard_addition_chain);
    add_test(suite, test_shunting_yard_unmatched_lparen);
    add_test(suite, test_shunting_yard_unmatched_rparen);

    return run_test_suite(suite, create_text_reporter());
}
