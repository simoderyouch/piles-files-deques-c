/* Exercice 17 - Maximum glissant avec une deque monotone
 * Cas A : T=[4,2,12,3,8,7,10,1], k=3 -> 12 12 12 8 10 10
 * Cas B : a=[4,2,12,11,-5,7],     k=3 -> 12 12 12 11
 * Etats de la deque (indices(valeurs)) pour B :
 *   i=0 : 0(4)            i=1 : 0(4) 1(2)          i=2 : 2(12)        sortie 12
 *   i=3 : 2(12) 3(11)  sortie 12
 *   i=4 : 2(12) 3(11) 4(-5)  sortie 12
 *   i=5 : l'indice 2 sort de la fenetre -> 3(11) 4(-5) ; 7 retire -5 puis
 *         s'arrete devant 11 > 7 -> 3(11) 5(7)    sortie 11
 * Naif : O(nk). Deque : O(n) car chaque indice entre une fois (push_back) et
 * sort au plus une fois (pop_back ou pop_front) : <= 2n operations.
 */
#include <stdio.h>
#include "deque.h"

static void show(const int *a, Deque *d)
{
    size_t n = deque_size(d);
    Item x;
    printf("deque:");
    for (size_t i = 0; i < n; i++) {
        deque_pop_front(d, &x); printf(" %d(%d)", x, a[x]); deque_push_back(d, x);
    }
}

static void naive(const int *a, size_t n, size_t k)
{
    printf("naif  :");
    for (size_t i = 0; i + k <= n; i++) {
        int m = a[i];
        for (size_t j = 1; j < k; j++) if (a[i + j] > m) m = a[i + j];
        printf(" %d", m);
    }
    printf("\n");
}

static void sliding(const int *a, size_t n, size_t k, int trace)
{
    Deque *d = deque_create();
    Item f, b;
    if (!trace) printf("deque :");
    for (size_t i = 0; i < n; i++) {
        if (deque_front(d, &f) == DS_OK && (size_t)f + k <= i) deque_pop_front(d, NULL);
        while (deque_back(d, &b) == DS_OK && a[b] <= a[i]) deque_pop_back(d, NULL);
        deque_push_back(d, (Item)i);
        if (trace) { printf(" i=%zu  ", i); show(a, d); printf("   "); }
        if (i + 1 >= k) { deque_front(d, &f); printf(trace ? "sortie %d" : " %d", a[f]); }
        if (trace) printf("\n");
    }
    if (!trace) printf("\n");
    deque_destroy(d);
}

int main(void)
{
    int A[] = {4, 2, 12, 3, 8, 7, 10, 1};
    int B[] = {4, 2, 12, 11, -5, 7};
    printf("Cas A (k=3)\n"); naive(A, 8, 3); sliding(A, 8, 3, 0);
    printf("Cas B (k=3)\n"); naive(B, 6, 3); sliding(B, 6, 3, 1);
    return 0;
}
