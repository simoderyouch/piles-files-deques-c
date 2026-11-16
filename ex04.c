/* Exercice 4 - Infixe -> postfixe (shunting-yard) : priorites et associativite
 *
 * Regle pour un operateur c : tant que le sommet t est un operateur avec
 *   prio(t) > prio(c)  OU  (prio(t) == prio(c) ET c associatif a GAUCHE)
 * on depile t vers la sortie ; puis on empile c.
 *
 * Resultats attendus :
 *   (A+B)*(C-D)/E   ->  A B + C D - * E /
 *   a*(b+c)-d/e     ->  a b c + * d e / -
 *   8-3-2           ->  8 3 - 2 -     (= 3, correct : - associatif a gauche)
 *   si - traite a DROITE : 8 3 2 - -  (= 8-(3-2) = 7, FAUX)
 *   2^3^2 (extension, ^ associatif a droite) -> 2 3 2 ^ ^  (= 2^9 = 512)
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#include "util.h"

static int prec(char c)
{
    switch (c) { case '+': case '-': return 1; case '*': case '/': return 2; case '^': return 3; default: return 0; }
}
static int is_op(char c) { return c != '\0' && strchr("+-*/^", c) != NULL; }
static int right_assoc(char c, int minus_right) { return c == '^' || (minus_right && c == '-'); }

/* retourne 0 si OK, -1 si parentheses mal formees / caractere inconnu */
static int to_postfix(const char *in, char *out, int minus_right, int trace)
{
    Stack *ops = stack_create();
    size_t k = 0;
    int rc = 0;
    for (const char *p = in; *p != '\0' && rc == 0; p++) {
        char c = *p;
        if (isspace((unsigned char)c)) continue;
        if (isalnum((unsigned char)c)) {
            out[k++] = c; out[k++] = ' ';
        } else if (c == '(') {
            stack_push(ops, c);
        } else if (c == ')') {
            Item t;
            for (;;) {
                if (stack_pop(ops, &t) == DS_EMPTY) { rc = -1; break; }   /* ')' sans '(' */
                if (t == '(') break;
                out[k++] = (char)t; out[k++] = ' ';
            }
        } else if (is_op(c)) {
            Item t;
            while (stack_top(ops, &t) == DS_OK && t != '(' &&
                   (prec((char)t) > prec(c) ||
                    (prec((char)t) == prec(c) && !right_assoc(c, minus_right)))) {
                stack_pop(ops, NULL);
                out[k++] = (char)t; out[k++] = ' ';
            }
            stack_push(ops, c);
        } else {
            rc = -1;
        }
        if (trace) {
            out[k] = '\0';
            printf("  '%c'  sortie = %-14s ", c, out);
            stack_print("ops", ops, 1);
        }
    }
    while (rc == 0 && !stack_is_empty(ops)) {
        Item t;
        stack_pop(ops, &t);
        if (t == '(') { rc = -1; break; }                                 /* '(' non fermee */
        out[k++] = (char)t; out[k++] = ' ';
    }
    out[k] = '\0';
    stack_destroy(ops);
    return rc;
}

/* evaluation d'un postfixe dont les operandes sont des chiffres (pour la demo) */
static int eval_digits(const char *post)
{
    Stack *s = stack_create();
    Item a, b, r = 0;
    for (const char *p = post; *p != '\0'; p++) {
        if (isdigit((unsigned char)*p)) { stack_push(s, *p - '0'); }
        else if (is_op(*p)) {
            stack_pop(s, &b); stack_pop(s, &a);
            switch (*p) {
            case '+': r = a + b; break; case '-': r = a - b; break;
            case '*': r = a * b; break; case '/': r = b ? a / b : 0; break;
            default: { r = 1; for (Item i = 0; i < b; i++) r *= a; }
            }
            stack_push(s, r);
        }
    }
    stack_pop(s, &r);
    stack_destroy(s);
    return r;
}

int main(void)
{
    const char *tests[] = { "(A+B)*(C-D)/E", "a*(b+c)-d/e", "8-3-2" };
    char out[128];
    for (size_t i = 0; i < 3; i++) {
        printf("--- %s\n", tests[i]);
        to_postfix(tests[i], out, 0, 1);
        printf("=> postfixe : %s\n", out);
    }
    to_postfix("8-3-2", out, 0, 0);
    printf("\n8-3-2, - associatif a gauche : %s = %d (correct)\n", out, eval_digits(out));
    to_postfix("8-3-2", out, 1, 0);
    printf("8-3-2, - traite a droite     : %s = %d (FAUX)\n", out, eval_digits(out));
    to_postfix("2^3^2", out, 0, 0);
    printf("2^3^2 (^ a droite)           : %s = %d\n", out, eval_digits(out));
    /* erreurs */
    printf("\nerreurs : \"(a+b\" -> %d, \"a+b)\" -> %d, \"a$b\" -> %d\n",
           to_postfix("(a+b", out, 0, 0), to_postfix("a+b)", out, 0, 0), to_postfix("a$b", out, 0, 0));
    return 0;
}
