#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include <stddef.h>
#include "ds_status.h"
typedef struct Queue Queue;
Queue *queue_create(void);
void queue_destroy(Queue *q);
bool queue_is_empty(const Queue *q);
size_t queue_size(const Queue *q);
DsStatus queue_enqueue(Queue *q, Item x);
DsStatus queue_dequeue(Queue *q, Item *out);
DsStatus queue_front(const Queue *q, Item *out);
#endif
