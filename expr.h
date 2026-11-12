#ifndef EXPR_H
#define EXPR_H
#include <stddef.h>
#include "ds_status.h"
#include "queue.h"

/* Convention lexicale : les espaces sont facultatifs. Un nombre est reconnu
 * par strtod et peut contenir un point decimal et un exposant.
 * Dans une expression, un '-' en position d'operande est normalise en
 * operateur unaire 'n'; un '+' unaire est ignore. La file lexicale contient
 * des indices vers tok[], car Queue stocke des Item (int), pas des Token. */
typedef enum { TK_NUM, TK_OP, TK_LP, TK_RP } TokKind;
typedef struct { TokKind kind; double val; char op; } Token;

/** Checks (), [] and {} nesting; err_pos receives the first error position. */
DsStatus expr_check_brackets(const char *s, size_t *err_pos);

/** Tokenizes s and appends token indices to lexemes. */
DsStatus expr_tokenize(const char *s, Token *tok, size_t max, size_t *n,
                       Queue *lexemes, size_t *err_pos);

/** Converts the token stream from infix notation to postfix notation. */
DsStatus expr_to_postfix(const Token *tok, size_t n, Queue *lexemes, Queue *out);

/** Evaluates a postfix token stream and writes the result to result. */
DsStatus expr_eval_postfix(const Token *tok, Queue *postfix, double *result);

/** Checks, parses and evaluates a complete arithmetic expression. */
DsStatus expr_eval(const char *s, double *result, size_t *err_pos);
#endif
