/*
 * src/lex.c
 * contains lexer and tokenization functions
 *
 * author: Ian Brault <ian.brault@engineering.ucla.edu>
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <lex.h>

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
    case '(':
        type = L_PAREN;
        break;
    case ')':
        type = R_PAREN;
        break;
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
static const char* get_literal(const char* c, literal* value)
{
    char* it;
    value->type = NONE;
    // attempt to get an integer literal
    int64_t val = strtol(c, &it, 10);
    // if it has not moved, not a valid literal
    if (it == c)
    {
        it = NULL;
    }
    // if strtol ends on a decimal point, the literal is a float
    else if (*it == '.')
    {
        value->value.f = strtod(c, &it);
        value->type = FLOAT;
    }
    // otherwise the literal was just an integer
    else
    {
        value->value.i = val;
        value->type = INT;
    }

    return it;
}

void init_token(token_t* token, token_type type, int32_t offset)
{
    *token = (token_t) { .type=type, .offset=offset };
}

void init_literal(token_t* token, literal value, int32_t offset)
{
    *token = (token_t) { .type=LITERAL, .value=value, .offset=offset };
}

static token_type binary_to_unary(token_type type)
{
    return type + N_BINARY_OPS;
}

// determine which + and - operators should be unary
static void add_unary_pos_neg_ops(token_t** tokens, int32_t n_tokens)
{
    if (!n_tokens)
    {
        return;
    }

    // addition/subtraction operator at the start should be unary
    if ((*tokens)[0].type == OP_ADD || (*tokens)[0].type == OP_SUB)
    {
        (*tokens)[0].type = binary_to_unary((*tokens)[0].type);
    }

    for (int i = 1; i < n_tokens; i++)
    {
        token_t* tcurr = &(*tokens)[i];
        token_t* tprev = &(*tokens)[i - 1];
        token_t* tnext = i + 1 < n_tokens ? &(*tokens)[i + 1] : NULL;
        // current token must be +/-
        uint8_t cond1 = tcurr->type == OP_ADD || tcurr->type == OP_SUB;
        // previous token must be a binary operator or a left parenthesis
        uint8_t cond2 = ARITY(*tprev) == 2 || tprev->type == L_PAREN;
        // next token must be a literal or a left parenthesis
        uint8_t cond3 = tnext && (IS_LITERAL(*tnext) || tnext->type == L_PAREN);
        if (cond1 && cond2 && cond3)
        {
            tcurr->type = binary_to_unary(tcurr->type);
        }
    }
}

token_t* tokenize(const char* input, int32_t* n_tokens)
{
    if (strlen(input) >= MAX_INPUT_LEN)
    {
        *n_tokens = E_MAX_INPUT;
        return NULL;
    }

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
        // note that parentheses are treated as operators here
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
        literal value;
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
    else
    {
        add_unary_pos_neg_ops(&tokens, *n_tokens);
    }


    return tokens;
}
