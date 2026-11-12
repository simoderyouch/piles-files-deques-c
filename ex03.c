/* Exercice 3 - Evaluation postfixee de reels signes
 *
 * Trace de  5 1 2 + 4 * + 3 -   (pile fond -> sommet) :
 *   5 : 5 | 1 : 5 1 | 2 : 5 1 2 | + : 5 3 | 4 : 5 3 4 | * : 5 12 | + : 17 | 3 : 17 3 | - : 14
 *
 * Trace de  125.75 -24.5 2.5 * + 3 /  :
 *   125.75              : 125.75
 *   -24.5 (nombre signe): 125.75 -24.5
 *   2.5                 : 125.75 -24.5 2.5
 *   *  (b=2.5, a=-24.5) : 125.75 -61.25
 *   +  (b=-61.25,a=125.75) : 64.5
 *   3                   : 64.5 3
 *   /  (b=3, a=64.5)    : 21.5
 *
 * Analyse lexicale : un jeton (separe par espaces) est un OPERATEUR s'il fait
 * exactement 1 caractere parmi + - * / ; sinon on tente strtod. Donc "-" seul
 * est un operateur, "-24.5" est un nombre signe.
 *
 * Limites de double : ~15-17 chiffres significatifs, 0.1 non representable
 * exactement, erreurs d'arrondi, depassement -> inf, comparaison a 0 delicate.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct { double *d; size_t n, cap; } DStack;
typedef enum { R_OK, R_UNDERFLOW, R_BADTOKEN, R_DIV0, R_BADFINAL, R_NOMEM } Rc;

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
static int dpop(DStack *s, double *x)
{
    if (s->n == 0) return -1;
    *x = s->d[--s->n];
    return 0;
}
static void dprint(const DStack *s)
{
    printf("  pile:");
    for (size_t i = 0; i < s->n; i++) printf(" %g", s->d[i]);
    printf("\n");
}

static Rc eval_rpn(const char *expr, double *result, int trace)
{
    size_t len = strlen(expr);
    char *copy = malloc(len + 1);
    DStack st = { NULL, 0, 0 };
    Rc rc = R_OK;
    if (copy == NULL) return R_NOMEM;
    memcpy(copy, expr, len + 1);

    for (char *tok = strtok(copy, " \t"); tok != NULL; tok = strtok(NULL, " \t")) {
        if (trace) printf(" jeton '%s'\n", tok);
        if (tok[1] == '\0' && strchr("+-*/", tok[0]) != NULL) {
            double a, b, r;
            if (dpop(&st, &b) != 0 || dpop(&st, &a) != 0) { rc = R_UNDERFLOW; break; }
            switch (tok[0]) {
            case '+': r = a + b; break;
            case '-': r = a - b; break;
            case '*': r = a * b; break;
            default:
                if (b == 0.0) { rc = R_DIV0; r = 0; } else r = a / b;
            }
            if (rc != R_OK) break;
            if (dpush(&st, r) != 0) { rc = R_NOMEM; break; }
        } else {
            char *end;
            double v = strtod(tok, &end);
            if (end == tok || *end != '\0' || !isfinite(v)) { rc = R_BADTOKEN; break; }
            if (dpush(&st, v) != 0) { rc = R_NOMEM; break; }
        }
        if (trace) dprint(&st);
    }
    if (rc == R_OK) {
        if (st.n != 1) rc = R_BADFINAL;   /* 0 ou plusieurs valeurs */
        else *result = st.d[0];
    }
    free(st.d);
    free(copy);
    return rc;
}

static const char *msg(Rc r)
{
    switch (r) {
    case R_OK: return "ok";
    case R_UNDERFLOW: return "sous-depilement (operandes insuffisants)";
    case R_BADTOKEN: return "jeton invalide";
    case R_DIV0: return "division par zero";
    case R_BADFINAL: return "pile finale de taille differente de 1";
    default: return "memoire";
    }
}

int main(void)
{
    const char *tests[] = {
        "5 1 2 + 4 * + 3 -", "12.5 3.2 +", "125.75 24.25 + 2 /",
        "10.5 -2.5 * 4 + ", "125.75 -24.5 2.5 * + 3 /",
        /* cas d'erreur */
        "1 +", "1 0 /", "1 2", "1 2 3 +", "abc", "1.5x 2 +", ""
    };
    for (size_t k = 0; k < sizeof tests / sizeof tests[0]; k++) {
        double r = 0;
        int trace = (k == 0 || k == 4);
        if (trace) printf("--- trace de \"%s\"\n", tests[k]);
        Rc rc = eval_rpn(tests[k], &r, trace);
        if (rc == R_OK) printf("\"%s\" = %g\n", tests[k], r);
        else printf("\"%s\" -> erreur : %s\n", tests[k], msg(rc));
    }
    return 0;
}
