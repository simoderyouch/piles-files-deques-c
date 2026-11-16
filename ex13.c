/* Exercice 13 - File avec deux piles (in, out)
 * enqueue : push dans in.
 * dequeue : si out est vide, on transfere TOUT in vers out (inverse l'ordre),
 *           puis on depile out.
 * Preuve FIFO : le transfert inverse l'ordre de in ; le plus ancien element
 * (au fond de in) arrive au sommet de out. Tant que out n'est pas vide, on
 * sert les elements deja transferes, tous plus anciens que ceux restant dans
 * in ; ils sortent dans l'ordre d'arrivee. Invariant : out (sommet->fond) puis
 * in (fond->sommet) = file dans l'ordre FIFO.
 * Amorti : un dequeue isole peut couter O(n) (transfert), mais chaque element
 * est empile/depile au plus 2 fois (in puis out) : m operations coutent au
 * plus 4m operations de pile -> O(m) au total, O(1) amorti par operation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "queue.h"

typedef struct { Stack *in, *out; long moves; } Q2;

static Q2 *q2_create(void)
{
    Q2 *q = malloc(sizeof *q);
    q->in = stack_create(); q->out = stack_create(); q->moves = 0;
    return q;
}
static void q2_destroy(Q2 *q) { stack_destroy(q->in); stack_destroy(q->out); free(q); }
static DsStatus q2_enqueue(Q2 *q, Item x) { q->moves++; return stack_push(q->in, x); }
static DsStatus q2_dequeue(Q2 *q, Item *out)
{
    if (stack_is_empty(q->out)) {
        Item t;
        while (stack_pop(q->in, &t) == DS_OK) { stack_push(q->out, t); q->moves += 2; }
    }
    q->moves++;
    return stack_pop(q->out, out);   /* DS_EMPTY si les deux piles sont vides */
}

int main(void)
{
    Q2 *q = q2_create();
    Item x;
    for (int i = 1; i <= 5; i++) q2_enqueue(q, i * 10);
    q2_dequeue(q, &x); printf("dequeue -> %d (transfert O(n))\n", x);
    q2_enqueue(q, 60);
    while (q2_dequeue(q, &x) == DS_OK) printf("dequeue -> %d\n", x);
    printf("dequeue a vide -> %s\n", q2_dequeue(q, &x) == DS_EMPTY ? "DS_EMPTY" : "?!");
    q2_destroy(q);

    /* Test aleatoire contre la vraie file : memes resultats, cout amorti */
    srand(1);
    Q2 *a = q2_create();
    Queue *b = queue_create();
    long m = 0;
    for (int i = 0; i < 100000; i++, m++) {
        if (rand() % 2) { q2_enqueue(a, i); queue_enqueue(b, i); }
        else {
            Item x1 = -1, x2 = -2;
            DsStatus s1 = q2_dequeue(a, &x1), s2 = queue_dequeue(b, &x2);
            assert(s1 == s2 && (s1 != DS_OK || x1 == x2));
        }
    }
    printf("100000 operations aleatoires identiques a la file de reference\n");
    printf("operations de pile : %ld pour m = %ld (<= 4m)\n", a->moves, m);
    assert(a->moves <= 4 * m);
    q2_destroy(a); queue_destroy(b);
    return 0;
}
