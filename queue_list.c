/* File : liste chainee avec head et tail (cours 4.4 B) */
#include <stdlib.h>
#include "queue.h"
typedef struct Node { Item value; struct Node *next; } Node;
struct Queue { Node *head; Node *tail; size_t size; };

Queue *queue_create(void)
{
    Queue *q = malloc(sizeof *q);
    if (q == NULL) return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}
void queue_destroy(Queue *q)
{
    if (q == NULL) return;
    Node *cur = q->head;
    while (cur != NULL) { Node *next = cur->next; free(cur); cur = next; }
    free(q);
}
bool queue_is_empty(const Queue *q) { return q->head == NULL; }
size_t queue_size(const Queue *q) { return q->size; }
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
DsStatus queue_front(const Queue *q, Item *out)
{
    if (q->head == NULL) return DS_EMPTY;
    *out = q->head->value;
    return DS_OK;
}
