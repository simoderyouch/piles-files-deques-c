/* Exercice 11 - Parcours en largeur (BFS)
 * Graphe non oriente (listes d'adjacence) :
 *   0:1,2   1:0,3,4   2:0,4   3:1,5   4:1,2,5   5:3,4
 * Depuis 0 : ordre 0 1 2 3 4 5 ; distances 0 1 1 2 2 3.
 * Justification O(V+E) : chaque sommet est enfile/defile au plus une fois
 * (marque a l'enfilage), et chaque liste d'adjacence est parcourue une fois :
 * somme des degres = 2E.
 * Correction : la file contient les sommets par distance croissante.
 */
#include <stdio.h>
#include "queue.h"

#define V 6
static const int deg[V] = {2, 3, 2, 2, 3, 2};
static const int adj[V][3] = { {1,2}, {0,3,4}, {0,4}, {1,5}, {1,2,5}, {3,4} };

int main(void)
{
    int dist[V];
    for (int i = 0; i < V; i++) dist[i] = -1;
    Queue *q = queue_create();
    Item u;
    dist[0] = 0;
    queue_enqueue(q, 0);
    printf("ordre de visite :");
    while (queue_dequeue(q, &u) == DS_OK) {
        printf(" %d", u);
        for (int i = 0; i < deg[u]; i++) {
            int v = adj[u][i];
            if (dist[v] < 0) { dist[v] = dist[u] + 1; queue_enqueue(q, v); }
        }
    }
    printf("\n");
    for (int i = 0; i < V; i++) printf("dist(0,%d) = %d\n", i, dist[i]);
    queue_destroy(q);
    return 0;
}
