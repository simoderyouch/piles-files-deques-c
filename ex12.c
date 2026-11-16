/* Exercice 12 - Generation des nombres binaires de 1 a n avec une file
 * Idee : file initiale ["1"]. Pour chaque etape : s = defiler ; afficher s ;
 * enfiler s+"0" puis s+"1" (les deux "enfants" de s).
 * n=6 : 1, 10, 11, 100, 101, 110.
 * Les chaines sont allouees dynamiquement : on libere s apres usage et on
 * libere aussi les chaines restantes dans la file a la fin.
 * Complexite : n etapes, chaque chaine a O(log n) caracteres -> O(n log n).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* file circulaire de char* de capacite fixe */
typedef struct { char **a; size_t cap, head, size; } SQ;

static int sq_init(SQ *q, size_t cap)
{
    q->a = malloc(cap * sizeof *q->a);
    q->cap = cap; q->head = q->size = 0;
    return q->a != NULL;
}
static int sq_enq(SQ *q, char *s)
{
    if (q->size == q->cap) return 0;
    q->a[(q->head + q->size++) % q->cap] = s;
    return 1;
}
static char *sq_deq(SQ *q)
{
    if (q->size == 0) return NULL;
    char *s = q->a[q->head];
    q->head = (q->head + 1) % q->cap;
    q->size--;
    return s;
}
static char *append(const char *s, char c)
{
    size_t n = strlen(s);
    char *r = malloc(n + 2);
    if (r == NULL) return NULL;
    memcpy(r, s, n);
    r[n] = c; r[n + 1] = '\0';
    return r;
}

int main(void)
{
    int n = 6;
    SQ q;
    if (!sq_init(&q, (size_t)n + 2)) return 1;
    char *one = malloc(2);
    one[0] = '1'; one[1] = '\0';
    sq_enq(&q, one);
    for (int i = 1; i <= n; i++) {
        char *s = sq_deq(&q);
        printf("%s%s", s, i < n ? ", " : "\n");
        char *s0 = append(s, '0'), *s1 = append(s, '1');
        if (s0 != NULL && !sq_enq(&q, s0)) free(s0);
        if (s1 != NULL && !sq_enq(&q, s1)) free(s1);
        free(s);
    }
    char *s;
    while ((s = sq_deq(&q)) != NULL) free(s);   /* chaines restantes */
    free(q.a);
    return 0;
}
