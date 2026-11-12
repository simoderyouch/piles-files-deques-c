#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "stack.h"

/* ---------- Etape 3 : parentheses ---------- */
/** Validates delimiter nesting with a stack of opening-character positions. */
DsStatus expr_check_brackets(const char *s, size_t *err_pos)
{
    Stack *st = stack_create();
    DsStatus res = DS_OK;
    size_t i;
    if (st == NULL) return DS_NOMEM;
    for (i = 0; s[i] != '\0' && res == DS_OK; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            if (stack_push(st, (Item)i) != DS_OK) res = DS_NOMEM;
        } else if (c == ')' || c == ']' || c == '}') {
            Item o;
            if (stack_pop(st, &o) == DS_EMPTY) { res = DS_SYNTAX; break; }  /* fermante orpheline */
            char oc = s[o];
            bool ok = (oc == '(' && c == ')') || (oc == '[' && c == ']') || (oc == '{' && c == '}');
            if (!ok) { res = DS_SYNTAX; break; }                              /* mauvais type */
        }
    }
    if (res == DS_OK && !stack_is_empty(st)) {                                /* ouvrante non fermee */
        Item o;
        stack_top(st, &o);
        i = (size_t)o;
        res = DS_SYNTAX;
    }
    if (res == DS_SYNTAX && err_pos != NULL) *err_pos = i;
    stack_destroy(st);
    return res;
}

/* ---------- Etape 4 : analyse lexicale ---------- */
/** Converts source characters into typed tokens and stores their indices. */
DsStatus expr_tokenize(const char *s, Token *tok, size_t max, size_t *n,
                       Queue *lexemes, size_t *err_pos)
{
    size_t k = 0;
    bool prev_operand = false;        /* vrai apres un nombre ou ')' */
    const char *p = s;
    while (*p != '\0') {
        char c = *p;
        size_t pos = (size_t)(p - s);
        if (isspace((unsigned char)c)) { p++; continue; }
        if (k >= max || k > (size_t)INT_MAX) { *err_pos = pos; return DS_BADARG; }
        if (isdigit((unsigned char)c) || c == '.') {
            char *end;
            double v;
            if (prev_operand) { *err_pos = pos; return DS_SYNTAX; }
            v = strtod(p, &end);
            if (end == p || !isfinite(v)) { *err_pos = pos; return DS_SYNTAX; }
            tok[k] = (Token){ TK_NUM, v, '\0' };
            if (queue_enqueue(lexemes, (Item)k) != DS_OK) return DS_NOMEM;
            k++;
            p = end;
            prev_operand = true;
            continue;
        }
        if (c == '(') {
            if (prev_operand) { *err_pos = pos; return DS_SYNTAX; }
            tok[k] = (Token){ TK_LP, 0, '(' };
            if (queue_enqueue(lexemes, (Item)k) != DS_OK) return DS_NOMEM;
            k++;
            prev_operand = false;
        } else if (c == ')') {
            if (!prev_operand) { *err_pos = pos; return DS_SYNTAX; }
            tok[k] = (Token){ TK_RP, 0, ')' };
            if (queue_enqueue(lexemes, (Item)k) != DS_OK) return DS_NOMEM;
            k++;
            prev_operand = true;
        } else if (strchr("+-*/^", c) != NULL) {
            if (prev_operand) {
                tok[k] = (Token){ TK_OP, 0, c };              /* operateur binaire */
                if (queue_enqueue(lexemes, (Item)k) != DS_OK) return DS_NOMEM;
                k++;
                prev_operand = false;
            } else if (c == '-') {
                tok[k] = (Token){ TK_OP, 0, 'n' };            /* moins unaire */
                if (queue_enqueue(lexemes, (Item)k) != DS_OK) return DS_NOMEM;
                k++;
            } else if (c != '+') {
                *err_pos = pos; return DS_SYNTAX;             /* ex. "*3" */
            }                                                  /* '+' unaire : ignore */
        } else {
            *err_pos = pos; return DS_SYNTAX;
        }
        p++;
    }
    if (!prev_operand) { *err_pos = (size_t)(p - s); return DS_SYNTAX; }   /* vide ou finit par un operateur */
    *n = k;
    return DS_OK;
}

/* ---------- Etape 5 : shunting-yard ---------- */
/** Returns the precedence used by the shunting-yard operator stack. */
static int prec(char op)
{
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case 'n': return 3;
    case '^': return 4;
    default:  return 0;
    }
}
/** Identifies operators that associate from right to left. */
static bool right_assoc(char op) { return op == '^'; }

/** Converts infix tokens to postfix tokens using an operator stack. */
DsStatus expr_to_postfix(const Token *tok, size_t n, Queue *lexemes, Queue *out)
{
    Stack *ops = stack_create();
    DsStatus st = DS_OK;
    Item t;
    Item i;
    if (ops == NULL) return DS_NOMEM;
    while (st == DS_OK && queue_dequeue(lexemes, &i) == DS_OK) {
        if (i < 0 || (size_t)i >= n) { st = DS_BADARG; break; }
        switch (tok[i].kind) {
        case TK_NUM:
            st = queue_enqueue(out, (Item)i);
            break;
        case TK_LP:
            st = stack_push(ops, (Item)i);
            break;
        case TK_RP:
            for (;;) {
                if (stack_pop(ops, &t) == DS_EMPTY) { st = DS_SYNTAX; break; }
                if (tok[t].kind == TK_LP) break;
                if ((st = queue_enqueue(out, t)) != DS_OK) break;
            }
            break;
        case TK_OP:
            if (tok[i].op != 'n') {      /* le prefixe 'n' ne depile rien */
                while (stack_top(ops, &t) == DS_OK && tok[t].kind == TK_OP &&
                       (prec(tok[t].op) > prec(tok[i].op) ||
                        (prec(tok[t].op) == prec(tok[i].op) && !right_assoc(tok[i].op)))) {
                    stack_pop(ops, NULL);
                    if ((st = queue_enqueue(out, t)) != DS_OK) break;
                }
            }
            if (st == DS_OK) st = stack_push(ops, (Item)i);
            break;
        }
    }
    while (st == DS_OK && stack_pop(ops, &t) == DS_OK) {
        if (tok[t].kind == TK_LP) st = DS_SYNTAX;       /* '(' non fermee */
        else st = queue_enqueue(out, t);
    }
    stack_destroy(ops);
    return st;
}

/* ---------- Etape 6 : evaluation avec pile de double ---------- */
typedef struct { double *d; size_t n, cap; } DStack;
/** Pushes a real value, growing the private evaluation stack if required. */
static int dpush(DStack *s, double x)
{
    if (s->n == s->cap) {
        size_t nc = s->cap ? s->cap * 2 : 8;
        if (s->cap > (size_t)-1 / 2 || nc > (size_t)-1 / sizeof *s->d)
            return -1;
        double *p = realloc(s->d, nc * sizeof *p);
        if (p == NULL) return -1;
        s->d = p; s->cap = nc;
    }
    s->d[s->n++] = x;
    return 0;
}

/** Evaluates postfix tokens, applying operators to the two top operands. */
DsStatus expr_eval_postfix(const Token *tok, Queue *postfix, double *result)
{
    DStack st = { NULL, 0, 0 };
    DsStatus rc = DS_OK;
    Item i;
    while (rc == DS_OK && queue_dequeue(postfix, &i) == DS_OK) {
        if (tok[i].kind == TK_NUM) {
            if (dpush(&st, tok[i].val) != 0) rc = DS_NOMEM;
        } else if (tok[i].op == 'n') {
            if (st.n < 1) { rc = DS_SYNTAX; break; }
            st.d[st.n - 1] = -st.d[st.n - 1];
        } else {
            double a, b, r;
            if (st.n < 2) { rc = DS_SYNTAX; break; }
            b = st.d[--st.n];            /* b est depile en premier */
            a = st.d[--st.n];
            switch (tok[i].op) {
            case '+': r = a + b; break;
            case '-': r = a - b; break;
            case '*': r = a * b; break;
            case '/': if (b == 0.0) { rc = DS_DIVZERO; r = 0; } else r = a / b; break;
            case '^': r = pow(a, b); break;
            default:  rc = DS_SYNTAX; r = 0;
            }
            if (rc == DS_OK && dpush(&st, r) != 0) rc = DS_NOMEM;
        }
    }
    if (rc == DS_OK) {
        if (st.n != 1) rc = DS_SYNTAX; else *result = st.d[0];
    }
    free(st.d);
    return rc;
}

/** Runs bracket checking, tokenization, conversion and evaluation as one API. */
DsStatus expr_eval(const char *s, double *result, size_t *err_pos)
{
    size_t n = 0, dummy = 0;
    size_t cap = strlen(s) + 1;
    DsStatus rc;
    Token *tok;
    Queue *lexemes;
    Queue *q;
    if (err_pos == NULL) err_pos = &dummy;
    if ((rc = expr_check_brackets(s, err_pos)) != DS_OK) return rc;
    tok = malloc(cap * sizeof *tok);
    if (tok == NULL) return DS_NOMEM;
    lexemes = queue_create();
    q = queue_create();
    if (lexemes == NULL || q == NULL) {
        queue_destroy(lexemes);
        queue_destroy(q);
        free(tok);
        return DS_NOMEM;
    }
    rc = expr_tokenize(s, tok, cap, &n, lexemes, err_pos);
    if (rc == DS_OK) rc = expr_to_postfix(tok, n, lexemes, q);
    if (rc == DS_OK) rc = expr_eval_postfix(tok, q, result);
    queue_destroy(lexemes);
    queue_destroy(q);
    free(tok);
    return rc;
}
