/* Exercice 14 - Simulation d'une deque
 * AddFirst(10) : 10
 * AddLast(20)  : 10 20
 * AddFirst(5)  : 5 10 20
 * RemoveLast() : -> 20 ; 5 10
 * AddLast(30)  : 5 10 30
 * AddFirst(2)  : 2 5 10 30
 * RemoveFirst(): -> 2 ; 5 10 30
 * Contenu final (tete -> queue) : 5 10 30.
 * Les 4 operations : deque_push_front / push_back / pop_front / pop_back (deque_circ.c).
 */
#include <stdio.h>
#include "deque.h"
#include "util.h"

int main(void)
{
    Deque *d = deque_create();
    Item x;
    deque_push_front(d, 10); deque_print("AddFirst(10)", d);
    deque_push_back(d, 20);  deque_print("AddLast(20) ", d);
    deque_push_front(d, 5);  deque_print("AddFirst(5) ", d);
    deque_pop_back(d, &x);   printf("RemoveLast -> %d\n", x); deque_print("            ", d);
    deque_push_back(d, 30);  deque_print("AddLast(30) ", d);
    deque_push_front(d, 2);  deque_print("AddFirst(2) ", d);
    deque_pop_front(d, &x);  printf("RemoveFirst -> %d\n", x); deque_print("            ", d);
    deque_destroy(d);
    return 0;
}
