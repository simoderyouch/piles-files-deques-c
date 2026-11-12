/* Deque : tableau circulaire (cours 5.3 A) */
#include <stdlib.h>
#include "deque.h"
#define DEQUE_INITIAL_CAPACITY 4
struct Deque { Item *data; size_t cap; size_t head; size_t size; };

Deque *deque_create(void)
{
    Deque *d = malloc(sizeof *d);
    if (d == NULL) return NULL;
    d->data = malloc(DEQUE_INITIAL_CAPACITY * sizeof *d->data);
    if (d->data == NULL) { free(d); return NULL; }
    d->cap = DEQUE_INITIAL_CAPACITY;
    d->head = 0;
    d->size = 0;
    return d;
}
void deque_destroy(Deque *d) { if (d == NULL) return; free(d->data); free(d); }
bool deque_is_empty(const Deque *d) { return d->size == 0; }
size_t deque_size(const Deque *d) { return d->size; }
static DsStatus grow(Deque *d)
{
    if (d->cap > (size_t)-1 / 2 ||
        d->cap * 2 > (size_t)-1 / sizeof *d->data)
        return DS_NOMEM;
    size_t new_cap = d->cap * 2;
    Item *p = malloc(new_cap * sizeof *p);
    if (p == NULL) return DS_NOMEM;
    for (size_t i = 0; i < d->size; i++) p[i] = d->data[(d->head + i) % d->cap];
    free(d->data);
    d->data = p;
    d->cap = new_cap;
    d->head = 0;
    return DS_OK;
}
DsStatus deque_push_back(Deque *d, Item x)
{
    if (d->size == d->cap) { DsStatus st = grow(d); if (st != DS_OK) return st; }
    d->data[(d->head + d->size) % d->cap] = x;
    d->size++;
    return DS_OK;
}
DsStatus deque_push_front(Deque *d, Item x)
{
    if (d->size == d->cap) { DsStatus st = grow(d); if (st != DS_OK) return st; }
    d->head = (d->head + d->cap - 1) % d->cap;   /* PAS (head-1)%cap : size_t ! */
    d->data[d->head] = x;
    d->size++;
    return DS_OK;
}
DsStatus deque_pop_front(Deque *d, Item *out)
{
    if (d->size == 0) return DS_EMPTY;
    if (out != NULL) *out = d->data[d->head];
    d->head = (d->head + 1) % d->cap;
    d->size--;
    return DS_OK;
}
DsStatus deque_pop_back(Deque *d, Item *out)
{
    if (d->size == 0) return DS_EMPTY;
    if (out != NULL) *out = d->data[(d->head + d->size - 1) % d->cap];
    d->size--;
    return DS_OK;
}
DsStatus deque_front(const Deque *d, Item *out)
{
    if (d->size == 0) return DS_EMPTY;
    *out = d->data[d->head];
    return DS_OK;
}
DsStatus deque_back(const Deque *d, Item *out)
{
    if (d->size == 0) return DS_EMPTY;
    *out = d->data[(d->head + d->size - 1) % d->cap];
    return DS_OK;
}
