/* Exercice 9 - File circulaire : traces, representations, capacite
 *
 * A) data[5], head=0, size=0, insertion en (head+size)%cap
 *   enqueue(1) : head=0 size=1 data=[1 . . . .]  file: 1
 *   enqueue(2) : head=0 size=2 data=[1 2 . . .]  file: 1 2
 *   enqueue(3) : head=0 size=3 data=[1 2 3 . .]  file: 1 2 3
 *   enqueue(4) : head=0 size=4 data=[1 2 3 4 .]  file: 1 2 3 4
 *   dequeue    : head=1 size=3 cases occupees 1..3  file: 2 3 4
 *   dequeue    : head=2 size=2 cases occupees 2..3  file: 3 4
 *   enqueue(5) : ecrit en (2+2)%5 = 4 ; head=2 size=3 ; file: 3 4 5
 *   enqueue(6) : ecrit en (2+3)%5 = 0 ; head=2 size=4 ; file: 3 4 5 6   (retour circulaire)
 *
 * B) Q[0..N-1], f = premier, r = prochaine case libre, une case sacrifiee, N = 8
 *   capacite utile = N-1 = 7
 *   taille = (N - f + r) mod N ; f=6, r=1 : (8-6+1) mod 8 = 3 elements (cases 6,7,0)
 *   f=3, r=2 : (r+1) mod N = 3 = f -> file PLEINE (7 elements)
 *
 * Differences : (head,size) -> capacite N, pas de case perdue, 1 champ de plus.
 * (f,r) -> capacite N-1 mais seulement 2 indices.
 * Agrandissement dynamique : si la file est "coupee en deux" (head+size > cap),
 * un realloc casse l'ordre ; il faut DEPLIER dans un nouveau tableau a partir
 * de l'indice 0 (voir grow() dans queue_circ.c).
 */
#include <stdio.h>
#include <stdbool.h>

#define CAP 5
typedef struct { int data[CAP]; size_t head, size; } QA;

static void qa_show(const QA *q)
{
    printf("  head=%zu size=%zu data=[", q->head, q->size);
    for (size_t i = 0; i < CAP; i++) {
        bool occ = false;
        for (size_t k = 0; k < q->size; k++) if ((q->head + k) % CAP == i) occ = true;
        if (occ) printf(" %d", q->data[i]); else printf(" .");
    }
    printf(" ]  file:");
    for (size_t k = 0; k < q->size; k++) printf(" %d", q->data[(q->head + k) % CAP]);
    printf("\n");
}
static bool qa_enq(QA *q, int x)
{
    if (q->size == CAP) return false;
    q->data[(q->head + q->size) % CAP] = x;
    q->size++;
    return true;
}
static bool qa_deq(QA *q, int *x)
{
    if (q->size == 0) return false;
    *x = q->data[q->head];
    q->head = (q->head + 1) % CAP;
    q->size--;
    return true;
}

/* B) representation f / r, une case sacrifiee */
#define N 8
typedef struct { int q[N]; size_t f, r; } QB;
static bool qb_empty(const QB *b) { return b->f == b->r; }
static bool qb_full(const QB *b)  { return (b->r + 1) % N == b->f; }
static size_t qb_size(const QB *b) { return (N - b->f + b->r) % N; }
static bool qb_enq(QB *b, int x) { if (qb_full(b)) return false; b->q[b->r] = x; b->r = (b->r + 1) % N; return true; }

int main(void)
{
    QA a = { {0}, 0, 0 };
    int x;
    printf("A)\n");
    for (int v = 1; v <= 4; v++) { qa_enq(&a, v); printf("enqueue(%d)", v); qa_show(&a); }
    qa_deq(&a, &x); printf("dequeue -> %d", x); qa_show(&a);
    qa_deq(&a, &x); printf("dequeue -> %d", x); qa_show(&a);
    qa_enq(&a, 5); printf("enqueue(5)"); qa_show(&a);
    qa_enq(&a, 6); printf("enqueue(6)"); qa_show(&a);
    printf("enqueue(7) -> %s\n", qa_enq(&a, 7) ? "ok" : "file pleine");

    printf("\nB) N=%d, capacite utile = %d\n", N, N - 1);
    QB b = { {0}, 6, 1 };
    printf("f=6 r=1 : taille = %zu\n", qb_size(&b));
    b.f = 3; b.r = 2;
    printf("f=3 r=2 : taille = %zu, pleine ? %s\n", qb_size(&b), qb_full(&b) ? "oui" : "non");
    QB c = { {0}, 0, 0 };
    int n = 0;
    while (qb_enq(&c, n)) n++;
    printf("file vide remplie jusqu'a saturation : %d elements (= N-1), vide ? %s\n", n, qb_empty(&c) ? "oui" : "non");
    return 0;
}
