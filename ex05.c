/* Exercice 5 - Pile d'appels et recursivite
 *
 * factorielle(5) recursive : descente 5 -> 4 -> 3 -> 2 -> 1 (cadres empiles),
 * remontee : 1, 2*1=2, 3*2=6, 4*6=24, 5*24=120 (cadres depiles).
 * Pile d'appels (sommet en haut) au point le plus profond :
 *   fact(1) <- fact(2) <- fact(3) <- fact(4) <- fact(5)
 *
 * Simulation par pile explicite : on empile n, n-1, ..., 2 puis on depile en
 * multipliant (cours 3.5).
 *
 * Pile explicite vs pile d'execution :
 *   - temps : O(n) dans les deux cas ; espace O(n) (cadres vs elements).
 *   - la pile d'execution est limitee (~8 Mio) -> stack overflow ;
 *     la pile explicite est sur le tas : limitee par la memoire seulement.
 *   - un cadre contient parametres+locales+adresse de retour : plus lourd
 *     qu'un simple entier empile.
 *
 * DFS : meme idee. En iteratif on empile les voisins dans l'ordre inverse
 * pour obtenir le meme ordre de visite que la version recursive.
 */
#include <stdio.h>
#include "stack.h"

static unsigned long fact_rec(unsigned n, int depth)
{
    printf("%*sappel fact(%u)\n", depth * 2, "", n);
    unsigned long r = (n <= 1) ? 1 : n * fact_rec(n - 1, depth + 1);
    printf("%*sretour fact(%u) = %lu\n", depth * 2, "", n, r);
    return r;
}
static unsigned long fact_stack(unsigned n)
{
    Stack *s = stack_create();
    unsigned long acc = 1;
    Item k;
    for (unsigned i = n; i > 1; i--) stack_push(s, (Item)i);
    while (stack_pop(s, &k) == DS_OK) acc *= (unsigned long)k;
    stack_destroy(s);
    return acc;
}

#define V 6
static const int adj[V][4] = { {1,2,-1,-1}, {0,3,-1,-1}, {0,3,4,-1}, {1,2,5,-1}, {2,-1,-1,-1}, {3,-1,-1,-1} };

static void dfs_rec(int v, int *visited)
{
    visited[v] = 1;
    printf(" %d", v);
    for (int i = 0; i < 4 && adj[v][i] >= 0; i++)
        if (!visited[adj[v][i]]) dfs_rec(adj[v][i], visited);
}
static void dfs_iter(int src)
{
    int visited[V] = {0};
    Stack *s = stack_create();
    Item v;
    stack_push(s, src);
    while (stack_pop(s, &v) == DS_OK) {
        if (visited[v]) continue;
        visited[v] = 1;
        printf(" %d", v);
        int deg = 0;
        while (deg < 4 && adj[v][deg] >= 0) deg++;
        for (int i = deg - 1; i >= 0; i--)          /* ordre inverse */
            if (!visited[adj[v][i]]) stack_push(s, adj[v][i]);
    }
    stack_destroy(s);
}

int main(void)
{
    printf("--- factorielle(5) recursive\n");
    unsigned long r = fact_rec(5, 0);
    printf("resultat = %lu ; version pile explicite = %lu\n\n", r, fact_stack(5));

    int visited[V] = {0};
    printf("DFS recursif  :"); dfs_rec(0, visited); printf("\n");
    printf("DFS iteratif  :"); dfs_iter(0);          printf("\n");
    printf("\nCouts : O(V+E) temps, O(V) espace de pile pour le DFS ; O(n) pour fact.\n");
    return 0;
}
