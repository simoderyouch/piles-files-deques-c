#ifndef UTIL_H
#define UTIL_H
/* Aides d'affichage pour les TD : on vide puis on remet les elements. */
#include <stdio.h>
#include "stack.h"
#include "queue.h"
#include "deque.h"

/* as_char = 1 : afficher les Item comme des caracteres */
static inline void stack_print(const char *label, Stack *s, int as_char)
{
    Stack *tmp = stack_create();
    Item x;
    while (stack_pop(s, &x) == DS_OK) stack_push(tmp, x);
    printf("%s[fond->sommet]:", label);
    while (stack_pop(tmp, &x) == DS_OK) {
        if (as_char) printf(" %c", (char)x); else printf(" %d", x);
        stack_push(s, x);
    }
    printf("\n");
    stack_destroy(tmp);
}
static inline void queue_print(const char *label, Queue *q)
{
    size_t n = queue_size(q);
    Item x;
    printf("%s[tete->queue]:", label);
    for (size_t i = 0; i < n; i++) {
        queue_dequeue(q, &x);
        printf(" %d", x);
        queue_enqueue(q, x);
    }
    printf("\n");
}
static inline void deque_print(const char *label, Deque *d)
{
    size_t n = deque_size(d);
    Item x;
    printf("%s[devant->derriere]:", label);
    for (size_t i = 0; i < n; i++) {
        deque_pop_front(d, &x);
        printf(" %d", x);
        deque_push_back(d, x);
    }
    printf("\n");
}
#endif
