/* Exercice 7 - Simulation d'une file
 * Enqueue(10) Enqueue(20) Enqueue(30) Dequeue() Enqueue(40) Dequeue() Enqueue(50)
 *   Enq 10 : 10          (tete=10, queue=10)
 *   Enq 20 : 10 20       (tete=10, queue=20)
 *   Enq 30 : 10 20 30    (tete=10, queue=30)
 *   Deq    : -> 10 ; 20 30
 *   Enq 40 : 20 30 40
 *   Deq    : -> 20 ; 30 40
 *   Enq 50 : 30 40 50    (tete=30, queue=50)
 * File finale : 30 40 50.
 */
#include <stdio.h>
#include "queue.h"
#include "util.h"

int main(void)
{
    Queue *q = queue_create();
    Item x;
    queue_enqueue(q, 10); queue_print("Enq 10", q);
    queue_enqueue(q, 20); queue_print("Enq 20", q);
    queue_enqueue(q, 30); queue_print("Enq 30", q);
    queue_dequeue(q, &x); printf("Deq -> %d\n", x); queue_print("      ", q);
    queue_enqueue(q, 40); queue_print("Enq 40", q);
    queue_dequeue(q, &x); printf("Deq -> %d\n", x); queue_print("      ", q);
    queue_enqueue(q, 50); queue_print("Enq 50", q);
    queue_front(q, &x);
    printf("front = %d, taille = %zu\n", x, queue_size(q));

    while (queue_dequeue(q, NULL) == DS_OK) { }
    printf("file vide ? %s\n", queue_is_empty(q) ? "oui" : "non");
    printf("dequeue a vide -> %s\n", queue_dequeue(q, &x) == DS_EMPTY ? "DS_EMPTY (detecte)" : "?!");
    printf("front a vide   -> %s\n", queue_front(q, &x) == DS_EMPTY ? "DS_EMPTY (detecte)" : "?!");
    queue_destroy(q);
    return 0;
}
