/* Exercice 18 - Plus courts chemins 0-1 BFS
 * Pour chaque arete (u,v,w), w dans {0,1} : si dist[u]+w < dist[v], on met a
 * jour dist[v] puis : w == 0 -> push_front(v), w == 1 -> push_back(v).
 * Correction : la deque reste triee par distance et ne contient que deux
 * valeurs consecutives d et d+1 (les d en tete, les d+1 en queue) ; c'est
 * l'invariant d'une file de priorite a 2 niveaux : on traite toujours un
 * sommet de distance minimale (comme Dijkstra).
 * Complexite : chaque sommet est reinsere au plus 2 fois (une distance ne
 * s'ameliore que d'une valeur parmi {d+1 -> d}) -> O(V+E).
 */
#include <stdio.h>
#include <limits.h>
#include "deque.h"

#define V 5
typedef struct { int to, w; } Edge;
/* graphe non oriente */
static const Edge adj[V][3] = {
    { {1,1}, {2,0} },          /* 0 */
    { {0,1}, {3,0} },          /* 1 */
    { {0,0}, {3,1}, {4,1} },   /* 2 */
    { {1,0}, {2,1}, {4,1} },   /* 3 */
    { {2,1}, {3,1} }           /* 4 */
};
static const int deg[V] = {2, 2, 3, 3, 2};

int main(void)
{
    int dist[V];
    for (int i = 0; i < V; i++) dist[i] = INT_MAX;
    Deque *d = deque_create();
    Item u;
    dist[0] = 0;
    deque_push_back(d, 0);
    while (deque_pop_front(d, &u) == DS_OK) {
        for (int i = 0; i < deg[u]; i++) {
            int v = adj[u][i].to, w = adj[u][i].w;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) deque_push_front(d, v); else deque_push_back(d, v);
            }
        }
    }
    for (int i = 0; i < V; i++) printf("dist(0,%d) = %d\n", i, dist[i]);
    /* attendu : 0 1 0 1 1 */
    deque_destroy(d);
    return 0;
}
