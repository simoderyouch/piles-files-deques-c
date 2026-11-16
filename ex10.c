/* Exercice 10 - Ordonnancement FIFO non preemptif
 * T1(0,4) T2(1,3) T3(2,5) T4(6,2)
 *   T1 : debut 0  fin 4   attente 0
 *   T2 : debut 4  fin 7   attente 3
 *   T3 : debut 7  fin 12  attente 5
 *   T4 : debut 12 fin 14  attente 6
 *   attente moyenne = (0+3+5+6)/4 = 3.5
 * Attente = debut - arrivee. Si la file est vide et qu'aucune tache n'est
 * arrivee, le processeur est inactif : on saute a la prochaine arrivee.
 */
#include <stdio.h>
#include "queue.h"

static void simulate(const int *arr, const int *dur, int n)
{
    Queue *ready = queue_create();
    int t = 0, next = 0, done = 0, total_wait = 0;
    Item id;
    while (done < n) {
        while (next < n && arr[next] <= t) queue_enqueue(ready, next++);   /* arrivees */
        if (queue_dequeue(ready, &id) != DS_OK) {                           /* inactivite */
            printf("  inactif de %d a %d\n", t, arr[next]);
            t = arr[next];
            continue;
        }
        int start = t, end = t + dur[id], wait = start - arr[id];
        printf("  T%d : debut %2d  fin %2d  attente %d\n", id + 1, start, end, wait);
        total_wait += wait;
        t = end;
        done++;
    }
    printf("  attente moyenne = %.2f\n", (double)total_wait / n);
    queue_destroy(ready);
}

int main(void)
{
    int arr[] = {0, 1, 2, 6}, dur[] = {4, 3, 5, 2};
    printf("Enonce :\n"); simulate(arr, dur, 4);
    int arr2[] = {0, 1, 20}, dur2[] = {2, 2, 1};
    printf("Avec periode d'inactivite :\n"); simulate(arr2, dur2, 3);
    return 0;
}
