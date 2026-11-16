/* Exercice 6 - Expression entierement parenthesee avec deux piles (Val, Op)
 *
 * ((2+3)*(6-2)) : apres chaque ')' :
 *   1re ')' : o=+, b=3, a=2 -> 5      Val: 5
 *   2e  ')' : o=-, b=2, a=6 -> 4      Val: 5 4
 *   3e  ')' : o=*, b=4, a=5 -> 20     Val: 20
 * (8-(3-2)) :
 *   1re ')' : o=-, b=2, a=3 -> 1      Val: 8 1
 *   2e  ')' : o=-, b=1, a=8 -> 7      Val: 7
 * Ordre de depilement : b (opérande DROIT) est depile en premier. Si on
 * inverse, (8-(3-2)) donne : 2-3=-1 puis -1-8 = -9 au lieu de 7.
 *
 * Pourquoi l'algorithme simplifie ne suffit pas pour 1+2*3 : il n'a aucune
 * parenthese qui declenche le calcul ; a la fin Val = 1 2 3 et Op = + * :
 * rien n'est evalue, et il n'a pas de regle de priorite (il faudrait
 * comparer prio(*) et prio(+) comme dans le shunting-yard).
 */
#include <stdio.h>
#include <ctype.h>
#include "stack.h"
#include "util.h"

static int apply(Stack *val, Stack *op, int wrong_order)
{
    Item o, a, b, r;
    if (stack_pop(op, &o) != DS_OK || stack_pop(val, &b) != DS_OK || stack_pop(val, &a) != DS_OK)
        return -1;
    if (wrong_order) { Item t = a; a = b; b = t; }
    switch (o) {
    case '+': r = a + b; break;
    case '-': r = a - b; break;
    case '*': r = a * b; break;
    case '/': if (b == 0) return -2; r = a / b; break;
    default: return -1;
    }
    stack_push(val, r);
    return 0;
}

static int eval(const char *s, int *result, int wrong_order, int trace)
{
    Stack *val = stack_create(), *op = stack_create();
    int rc = 0;
    for (const char *p = s; *p != '\0' && rc == 0; p++) {
        if (isspace((unsigned char)*p)) continue;
        if (isdigit((unsigned char)*p)) {
            int n = 0;
            while (isdigit((unsigned char)*p)) { n = n * 10 + (*p - '0'); p++; }
            p--;
            stack_push(val, n);
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            stack_push(op, *p);
        } else if (*p == ')') {
            rc = apply(val, op, wrong_order);
            if (trace && rc == 0) { stack_print("  Val", val, 0); stack_print("  Op ", op, 1); }
        } else if (*p != '(') {
            rc = -1;
        }
    }
    if (rc == 0 && (stack_size(val) != 1 || !stack_is_empty(op))) rc = -1;
    if (rc == 0) stack_top(val, result);
    stack_destroy(val); stack_destroy(op);
    return rc;
}

int main(void)
{
    int r;
    printf("((2+3)*(6-2))\n");
    if (eval("((2+3)*(6-2))", &r, 0, 1) == 0) printf("= %d\n\n", r);
    printf("(8-(3-2))\n");
    if (eval("(8-(3-2))", &r, 0, 1) == 0) printf("= %d (correct)\n", r);
    if (eval("(8-(3-2))", &r, 1, 0) == 0) printf("ordre inverse : %d (FAUX)\n\n", r);
    printf("1+2*3 -> %s\n", eval("1+2*3", &r, 0, 0) != 0 ? "erreur : l'algorithme simplifie ne suffit pas" : "?");
    printf("(1+ -> %s\n", eval("(1+", &r, 0, 0) != 0 ? "erreur detectee" : "?");
    printf("(4/(2-2)) -> %s\n", eval("(4/(2-2))", &r, 0, 0) == -2 ? "division par zero" : "?");
    return 0;
}
