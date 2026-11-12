/* File : liste chainee avec head et tail (cours 4.4 B) */
#include <stdlib.h>
#include "queue.h"
typedef struct Node { Item value; struct Node *next; } Node;
struct Queue { Node *head; Node *tail; size_t size; };

/** Allocates an empty linked queue with head and tail set to NULL. */
Queue *queue_create(void)
{
    Queue *q = malloc(sizeof *q);
    if (q == NULL) return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}
/** Frees all nodes in FIFO order and then the queue object. */
void queue_destroy(Queue *q)
{
    if (q == NULL) return;
    Node *cur = q->head;
    while (cur != NULL) { Node *next = cur->next; free(cur); cur = next; }
    free(q);
}
/** Tests whether the head pointer is NULL. */
bool queue_is_empty(const Queue *q) { return q->head == NULL; }
/** Returns the number of linked nodes. */
size_t queue_size(const Queue *q) { return q->size; }
/** Allocates a node and attaches it after the current tail. */
DsStatus queue_enqueue(Queue *q, Item x)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) return DS_NOMEM;
    n->value = x;
    n->next = NULL;
    if (q->tail == NULL) q->head = n; else q->tail->next = n;
    q->tail = n;
    q->size++;
    return DS_OK;
}
/** Removes the head node and repairs tail when the queue becomes empty. */
DsStatus queue_dequeue(Queue *q, Item *out)
{
    if (q->head == NULL) return DS_EMPTY;
    Node *n = q->head;
    if (out != NULL) *out = n->value;
    q->head = n->next;
    if (q->head == NULL) q->tail = NULL;   /* piege : ne pas oublier */
    free(n);
    q->size--;
    return DS_OK;
}
/** Reads the head value without removing its node. */
DsStatus queue_front(const Queue *q, Item *out)
{
    if (q->head == NULL) return DS_EMPTY;
    *out = q->head->value;
    return DS_OK;
}
