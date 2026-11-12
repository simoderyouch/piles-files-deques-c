#ifndef DEQUE_H
#define DEQUE_H
#include <stdbool.h>
#include <stddef.h>
#include "ds_status.h"
typedef struct Deque Deque;

/** Creates an empty double-ended queue. @return NULL on allocation failure. */
Deque *deque_create(void);

/** Releases the deque and all owned storage. NULL is accepted. */
void deque_destroy(Deque *d);

/** @return true when the deque contains no item. */
bool deque_is_empty(const Deque *d);

/** @return The number of items currently stored in the deque. */
size_t deque_size(const Deque *d);

/** Inserts x at the front. @return DS_OK or DS_NOMEM. */
DsStatus deque_push_front(Deque *d, Item x);

/** Inserts x at the back. @return DS_OK or DS_NOMEM. */
DsStatus deque_push_back(Deque *d, Item x);

/** Removes the front item and optionally stores it in out. */
DsStatus deque_pop_front(Deque *d, Item *out);

/** Removes the back item and optionally stores it in out. */
DsStatus deque_pop_back(Deque *d, Item *out);

/** Reads the front item without removing it. */
DsStatus deque_front(const Deque *d, Item *out);

/** Reads the back item without removing it. */
DsStatus deque_back(const Deque *d, Item *out);
#endif
