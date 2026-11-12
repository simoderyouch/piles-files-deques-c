#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "expr.h"

/** Asserts that expression s evaluates to the expected real value. */
static void ok(const char *s, double expected)
{
    double r = 0;
    size_t pos = 0;
    DsStatus st = expr_eval(s, &r, &pos);
    if (st != DS_OK || fabs(r - expected) > 1e-9) {
        printf("ECHEC : \"%s\" st=%d r=%g attendu=%g\n", s, (int)st, r, expected);
        assert(0);
    }
}
/** Asserts that expression s fails with the expected status. */
static void err(const char *s, DsStatus expected)
{
    double r = 0;
    size_t pos = 0;
    DsStatus st = expr_eval(s, &r, &pos);
    if (st != expected) { printf("ECHEC : \"%s\" st=%d attendu=%d\n", s, (int)st, (int)expected); assert(0); }
}

/** Runs lexical, bracket, precedence, unary-operator and error tests. */
int main(void)
{
    size_t pos = 99;
    Token lexed[16];
    size_t lexed_n = 0;
    Item index;
    Queue *lexemes = queue_create();
    assert(lexemes != NULL);
    assert(expr_tokenize("12.5 + -3", lexed, 16, &lexed_n, lexemes, &pos) == DS_OK);
    assert(lexed_n == 4 && queue_size(lexemes) == 4);
    assert(queue_dequeue(lexemes, &index) == DS_OK && lexed[index].kind == TK_NUM);
    assert(queue_dequeue(lexemes, &index) == DS_OK && lexed[index].op == '+');
    assert(queue_dequeue(lexemes, &index) == DS_OK && lexed[index].op == 'n');
    assert(queue_dequeue(lexemes, &index) == DS_OK && lexed[index].kind == TK_NUM);
    queue_destroy(lexemes);

    /* parenthesage */
    assert(expr_check_brackets("{[()]}", &pos) == DS_OK);
    assert(expr_check_brackets("([)]", &pos) == DS_SYNTAX && pos == 2);
    assert(expr_check_brackets("(()", &pos) == DS_SYNTAX && pos == 0);
    assert(expr_check_brackets("a+b)", &pos) == DS_SYNTAX && pos == 3);

    /* priorites et associativite */
    ok("3+4*2", 11);
    ok("(3+4)*2", 14);
    ok("10-4-3", 3);
    ok("8-3-2", 3);
    ok("(8-(3-2))", 7);
    ok("2^3^2", 512);
    ok("100/10/5", 2);
    /* reels */
    ok("12.5+3.2", 15.7);
    ok("125.75+24.25", 150);
    ok("0.1*3", 0.3);
    ok("125.75 + -24.5*2.5", 64.5);
    /* moins unaire */
    ok("-3+4", 1);
    ok("2*-(1+1)", -4);
    ok("-2^2", -4);
    ok("2^-1", 0.5);
    ok("--3", 3);
    /* erreurs */
    err("1/0", DS_DIVZERO);
    err("1e309", DS_SYNTAX);
    err("nan", DS_SYNTAX);
    err("3 4", DS_SYNTAX);
    err("3+", DS_SYNTAX);
    err("+*3", DS_SYNTAX);
    err("", DS_SYNTAX);
    err("()", DS_SYNTAX);
    err("(1+2", DS_SYNTAX);
    err("1+2)", DS_SYNTAX);
    err("1+a", DS_SYNTAX);
    err("2(3)", DS_SYNTAX);
    printf("test_expr : OK\n");
    return 0;
}
