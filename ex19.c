/* Exercice 19 - Pieges de programmation en C
 *
 * 1) Deque circulaire indexee par size_t : si head == 0, (head-1)%cap vaut
 *    SIZE_MAX % cap (sous-depassement non signe, PAS une erreur du
 *    compilateur) et non cap-1. Correction : (head + cap - 1) % cap.
 *
 * 2) File chainee : oublier tail = NULL quand on retire le dernier element.
 *    head devient NULL mais tail pointe vers un noeud LIBERE (pointeur
 *    pendant). Le prochain enqueue voit tail != NULL et ecrit tail->next
 *    dans de la memoire liberee : comportement indefini, corruption du tas,
 *    file qui parait vide alors qu'elle contient un element, etc.
 *    Test revelateur : enfiler, defiler (file vide), puis verifier
 *    l'invariant "head == NULL <=> tail == NULL" et re-enfiler.
 *    Ici le test verifie l'invariant SANS reenfiler dans la version fautive
 *    (pour ne pas executer de comportement indefini) ; avec AddressSanitizer
 *    le reenfilage fautif donnerait heap-use-after-free.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node { int v; struct Node *next; } Node;
typedef struct { Node *head, *tail; size_t size; } LQ;

static int enq(LQ *q, int x)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) return -1;
    n->v = x; n->next = NULL;
    if (q->tail == NULL) q->head = n; else q->tail->next = n;
    q->tail = n;
    q->size++;
    return 0;
}
static int deq_buggy(LQ *q, int *x)      /* DEFAUT : tail jamais remis a NULL */
{
    if (q->head == NULL) return -1;
    Node *n = q->head;
    *x = n->v;
    q->head = n->next;
    free(n);
    q->size--;
    return 0;
}
static int deq_ok(LQ *q, int *x)
{
    if (q->head == NULL) return -1;
    Node *n = q->head;
    *x = n->v;
    q->head = n->next;
    if (q->head == NULL) q->tail = NULL; /* correction */
    free(n);
    q->size--;
    return 0;
}
static int invariant(const LQ *q) { return (q->head == NULL) == (q->tail == NULL) && (q->head == NULL) == (q->size == 0); }

int main(void)
{
    /* test "vider puis reenfiler" */
    LQ bad = { NULL, NULL, 0 }, good = { NULL, NULL, 0 };
    int x;
    enq(&bad, 1);  deq_buggy(&bad, &x);
    enq(&good, 1); deq_ok(&good, &x);
    printf("version fautive : invariant apres vidage = %s\n", invariant(&bad) ? "ok" : "VIOLE -> tail pendant (defaut detecte)");
    printf("version corrigee: invariant apres vidage = %s\n", invariant(&good) ? "ok" : "viole");
    enq(&good, 2); enq(&good, 3);
    assert(good.size == 2 && invariant(&good));
    deq_ok(&good, &x); assert(x == 2);
    deq_ok(&good, &x); assert(x == 3);
    assert(invariant(&good));
    printf("vider puis reenfiler (version corrigee) : OK\n");
    /* pas de free(bad) : tail est pendant ; head est NULL donc rien a liberer */

    size_t head = 0, cap = 5;   /* attention : avec cap = 8 (puissance de 2) le bug est MASQUE */
    printf("\nsize_t : (0-1)%%5 = %zu au lieu de 4 ; (0+5-1)%%5 = %zu\n", (head - 1) % cap, (head + cap - 1) % cap);
    return 0;
}
