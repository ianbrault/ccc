/*
 * src/lex.c
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <ctype.h>
#include <stdlib.h>

#include <error.h>
#include <lex.h>

uint8_t is_operator(token_type type)
{
    return (type == OP_ADD) || (type == OP_SUB) || (type == OP_MUL);
}

/*
 * skip all leading/trailing whitespace
 */
static inline const char* skip_whitespace(const char* c)
{
    const char* it = c;
    while (isspace(*it))
    {
        it++;
    }
    return it;
}

/*
 * attempt to get the token_type for an operator
 * if the character is not an operator, return INVALID
 */
static token_type get_operator_type(const char* c)
{
    token_type type;
    switch (*c)
    {
    case '+':
        type = OP_ADD;
        break;
    case '-':
        type = OP_SUB;
        break;
    case '*':
        type = OP_MUL;
        break;
    default:
        type = INVALID;
    }
    return type;
}

/*
 * attempt to get a literal (passed as an oparam)
 * return the end position of the literal if successful, or return NULL if the
 * string passed is not a literal
 */
static const char* get_literal(const char* c, int32_t* value)
{
    char* it;
    int32_t val = strtol(c, &it, 10);
    // if it has not moved, not a valid literal
    if (it == c)
    {
        it = NULL;
    }
    // otherwise get the value of the parsed literal
    else
    {
        *value = val;
    }

    return it;
}

/*
 * initialize a token (not a literal) with a given type and offset
 */
static void init_token(token_t* token, token_type type, int32_t offset)
{
    *token = (token_t) { .type=type, .value=0, .offset=offset };
}

/*
 * initialize a literal with a given value and offset
 */
static void init_literal(token_t* token, int32_t value, int32_t offset)
{
    *token = (token_t) { .type=LITERAL, .value=value, .offset=offset };
}

token_t* tokenize(const char* input, int32_t* n_tokens)
{
    token_t* tokens = malloc(MAX_TOKENS * sizeof(token_t));
    *n_tokens = 0;
    const char* it = input;

    while (it && *it)
    {
        // skip leading whitspace
        it = skip_whitespace(it);
        // all trailing whitespace consumed
        if (!(*it))
        {
            break;
        }
        int32_t offset = it - input;

        // attempt to get an operator type
        token_type type = get_operator_type(it);
        if (type != INVALID)
        {
            init_token(&tokens[*n_tokens], type, offset);
            it++;
            *n_tokens = *n_tokens + 1;
            // error out if max number of tokens has been exceeded
            if (*n_tokens == MAX_TOKENS)
            {
                *n_tokens = E_MAX_TOKENS;
                it = 0;
            }
            continue;
        }

        // otherwise attempt to get a literal
        int32_t value;
        it = get_literal(it, &value);
        if (it)
        {
            init_literal(&tokens[*n_tokens], value, offset);
            *n_tokens = *n_tokens + 1;
            // error out if max number of tokens has been exceeded
            if (*n_tokens == MAX_TOKENS)
            {
                *n_tokens = E_MAX_TOKENS;
                it = 0;
            }
            continue;
        }

        // if this point has been reached, token is invalid
        *n_tokens = E_INVALID_TOKEN | offset;
        it = 0;
    }

    // if an error has been encountered, deallocate tokens array
    if (!it)
    {
        free(tokens);
        tokens = NULL;
    }

    return tokens;
}
