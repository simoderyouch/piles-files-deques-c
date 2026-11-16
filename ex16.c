/* Exercice 16 - Deque circulaire a capacite fixe
 * Invariants : 0 <= head < cap ; 0 <= size <= cap ;
 *   les elements occupent les cases (head+i)%cap, 0 <= i < size.
 *   vide : size == 0 ; plein : size == cap.
 *   devant = data[head] ; derriere = data[(head+size-1)%cap].
 * addFirst : head <- (head + cap - 1) % cap puis ecriture.
 * Pourquoi (head-1)%cap est faux : head est un size_t ; si head == 0,
 * head-1 = SIZE_MAX (2^64-1) et SIZE_MAX % cap n'est pas cap-1 en general.
 * Correction : (head + cap - 1) % cap (cap > 0).
 * (Si cap est une puissance de 2, le bug est masque : 2^64-1 mod 2^k = 2^k-1.)
 * Toutes les operations sont O(1) (aucune boucle, aucun decalage).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#define CAP 5
typedef struct { int data[CAP]; size_t head, size; } DQ;

static bool dq_empty(const DQ *d) { return d->size == 0; }
static bool dq_full(const DQ *d)  { return d->size == CAP; }
static bool add_last(DQ *d, int x)
{
    if (dq_full(d)) return false;
    d->data[(d->head + d->size) % CAP] = x;
    d->size++;
    return true;
}
static bool add_first(DQ *d, int x)
{
    if (dq_full(d)) return false;
    d->head = (d->head + CAP - 1) % CAP;
    d->data[d->head] = x;
    d->size++;
    return true;
}
static bool remove_first(DQ *d, int *x)
{
    if (dq_empty(d)) return false;
    *x = d->data[d->head];
    d->head = (d->head + 1) % CAP;
    d->size--;
    return true;
}
static bool remove_last(DQ *d, int *x)
{
    if (dq_empty(d)) return false;
    *x = d->data[(d->head + d->size - 1) % CAP];
    d->size--;
    return true;
}
static bool get_first(const DQ *d, int *x) { if (dq_empty(d)) return false; *x = d->data[d->head]; return true; }
static bool get_last(const DQ *d, int *x)  { if (dq_empty(d)) return false; *x = d->data[(d->head + d->size - 1) % CAP]; return true; }

int main(void)
{
    DQ d = { {0}, 0, 0 };
    int x;
    /* cas head = 0 : addFirst doit boucler vers la case CAP-1 */
    assert(d.head == 0);
    add_first(&d, 1);
    assert(d.head == CAP - 1);
    printf("addFirst avec head=0 -> head=%zu (= cap-1)\n", d.head);
    add_last(&d, 2); add_first(&d, 0); add_last(&d, 3); add_first(&d, -1);
    assert(dq_full(&d));
    assert(!add_first(&d, 99) && !add_last(&d, 99));
    get_first(&d, &x); assert(x == -1);
    get_last(&d, &x);  assert(x == 3);
    remove_first(&d, &x); assert(x == -1);
    remove_last(&d, &x);  assert(x == 3);
    remove_last(&d, &x);  assert(x == 2);
    remove_first(&d, &x); assert(x == 0);
    remove_first(&d, &x); assert(x == 1);
    assert(dq_empty(&d) && !remove_first(&d, &x) && !remove_last(&d, &x));
    printf("tests d'ordre, plein, vide : OK\n");

    /* demonstration du piege */
    size_t head = 0;
    volatile size_t cap = 5;
    printf("\nPiege size_t, head=0, cap=5 :\n");
    printf("  (head-1)%%cap        = %zu  (FAUX, attendu 4)\n", (head - 1) % cap);
    printf("  (head+cap-1)%%cap    = %zu  (correct)\n", (head + cap - 1) % cap);
    cap = 6;
    printf("avec cap=6 : (head-1)%%cap = %zu (FAUX, attendu 5)\n", (head - 1) % cap);
    return 0;
}
