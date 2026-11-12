#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include <stddef.h>
#include "ds_status.h"
typedef struct Queue Queue;

/** Creates an empty FIFO queue. @return NULL when allocation fails. */
Queue *queue_create(void);

/** Releases the queue and all storage owned by it. NULL is accepted. */
void queue_destroy(Queue *q);

/** @return true when the queue contains no item. */
bool queue_is_empty(const Queue *q);

/** @return The number of items currently stored in the queue. */
size_t queue_size(const Queue *q);

/** Adds an item at the rear of the queue.
 *  @return DS_OK, or DS_NOMEM when storage cannot be allocated.
 */
DsStatus queue_enqueue(Queue *q, Item x);

/** Removes the item at the front and optionally stores it in out.
 *  @return DS_OK, or DS_EMPTY when the queue is empty.
 */
DsStatus queue_dequeue(Queue *q, Item *out);

/** Reads the front item without removing it.
 *  @return DS_OK, or DS_EMPTY when the queue is empty.
 */
DsStatus queue_front(const Queue *q, Item *out);
#endif
