#ifndef DEQUE_H
#define DEQUE_H
#include <stdbool.h>
#include <stddef.h>
#include "ds_status.h"
typedef struct Deque Deque;
Deque *deque_create(void);
void deque_destroy(Deque *d);
bool deque_is_empty(const Deque *d);
size_t deque_size(const Deque *d);
DsStatus deque_push_front(Deque *d, Item x);
DsStatus deque_push_back(Deque *d, Item x);
DsStatus deque_pop_front(Deque *d, Item *out);
DsStatus deque_pop_back(Deque *d, Item *out);
DsStatus deque_front(const Deque *d, Item *out);
DsStatus deque_back(const Deque *d, Item *out);
#endif
