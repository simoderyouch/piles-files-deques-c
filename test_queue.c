/* Meme test lie aux deux implementations (queue_circ.c et queue_list.c) */
#include <assert.h>
#include <stdio.h>
#include "queue.h"

/** Exercises wrapping, growth, reuse and empty-queue behavior. */
int main(void)
{
    Queue *q = queue_create();
    Item x = 777;
    assert(queue_is_empty(q));
    assert(queue_dequeue(q, &x) == DS_EMPTY && x == 777);
    assert(queue_front(q, &x) == DS_EMPTY);
    /* enroulement + agrandissement d'une file "coupee en deux" */
    for (int i = 1; i <= 3; i++) queue_enqueue(q, i);
    queue_dequeue(q, &x); assert(x == 1);
    queue_dequeue(q, &x); assert(x == 2);
    for (int i = 4; i <= 20; i++) queue_enqueue(q, i);     /* head != 0, puis croissance */
    for (int i = 3; i <= 20; i++) { assert(queue_front(q, &x) == DS_OK && x == i); queue_dequeue(q, &x); assert(x == i); }
    assert(queue_is_empty(q));
    /* vider puis reutiliser (piege tail = NULL) */
    queue_enqueue(q, 42);
    queue_dequeue(q, &x);
    queue_enqueue(q, 43); queue_enqueue(q, 44);
    assert(queue_size(q) == 2);
    queue_dequeue(q, &x); assert(x == 43);
    queue_dequeue(q, &x); assert(x == 44);
    assert(queue_is_empty(q));
    queue_enqueue(q, 1);
    queue_destroy(q);
    queue_destroy(NULL);
    printf("test_queue : OK\n");
    return 0;
}
