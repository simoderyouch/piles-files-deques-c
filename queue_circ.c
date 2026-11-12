/* File : tableau circulaire avec compteur size (cours 4.4 A) */
#include <stdlib.h>
#include "queue.h"
#define QUEUE_INITIAL_CAPACITY 4
struct Queue { Item *data; size_t cap; size_t head; size_t size; };

/** Allocates an empty queue and its initial circular buffer. */
Queue *queue_create(void)
{
    Queue *q = malloc(sizeof *q);
    if (q == NULL) return NULL;
    q->data = malloc(QUEUE_INITIAL_CAPACITY * sizeof *q->data);
    if (q->data == NULL) { free(q); return NULL; }
    q->cap = QUEUE_INITIAL_CAPACITY;
    q->head = 0;
    q->size = 0;
    return q;
}
/** Releases the circular buffer and queue object. */
void queue_destroy(Queue *q) { if (q == NULL) return; free(q->data); free(q); }
/** Tests whether the queue contains no elements. */
bool queue_is_empty(const Queue *q) { return q->size == 0; }
/** Returns the number of elements in FIFO order. */
size_t queue_size(const Queue *q) { return q->size; }

/* On "deplie" l'anneau : un realloc simple casserait l'ordre. */
/** Doubles capacity while copying logical FIFO order from the ring. */
static DsStatus grow(Queue *q)
{
    if (q->cap > (size_t)-1 / 2 ||
        q->cap * 2 > (size_t)-1 / sizeof *q->data)
        return DS_NOMEM;
    size_t new_cap = q->cap * 2;
    Item *p = malloc(new_cap * sizeof *p);
    if (p == NULL) return DS_NOMEM;
    for (size_t i = 0; i < q->size; i++) p[i] = q->data[(q->head + i) % q->cap];
    free(q->data);
    q->data = p;
    q->cap = new_cap;
    q->head = 0;
    return DS_OK;
}
/** Writes x after the logical tail, growing the ring if necessary. */
DsStatus queue_enqueue(Queue *q, Item x)
{
    if (q->size == q->cap) {
        DsStatus st = grow(q);
        if (st != DS_OK) return st;
    }
    q->data[(q->head + q->size) % q->cap] = x;
    q->size++;
    return DS_OK;
}
/** Removes the logical head and advances the ring head index. */
DsStatus queue_dequeue(Queue *q, Item *out)
{
    if (q->size == 0) return DS_EMPTY;
    if (out != NULL) *out = q->data[q->head];
    q->head = (q->head + 1) % q->cap;
    q->size--;
    return DS_OK;
}
/** Reads the logical head without removing it. */
DsStatus queue_front(const Queue *q, Item *out)
{
    if (q->size == 0) return DS_EMPTY;
    *out = q->data[q->head];
    return DS_OK;
}
