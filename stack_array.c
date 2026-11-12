/* Pile : tableau dynamique (cours 2.3 A) */
#include "stack.h"
#include <assert.h>
#include <stdlib.h>
#define STACK_INITIAL_CAPACITY 8

static_assert(STACK_INITIAL_CAPACITY > 0, "capacite initiale > 0");

struct Stack {
  Item *data;
  size_t size;
  size_t cap;
};

/** Allocates an empty stack with a small initial dynamic array. */
Stack *stack_create(void) {
  Stack *s = malloc(sizeof *s);
  if (s == NULL)
    return NULL;
  s->data = malloc(STACK_INITIAL_CAPACITY * sizeof *s->data);
  if (s->data == NULL) {
    free(s);
    return NULL;
  }
  s->size = 0;
  s->cap = STACK_INITIAL_CAPACITY;
  return s;
}
/** Frees the array and the stack object. */
void stack_destroy(Stack *s) {
  if (s == NULL)
    return;
  free(s->data);
  free(s);
}
/** Tests whether no item is stored. */
bool stack_is_empty(const Stack *s) { return s->size == 0; }
/** Returns the current number of stored items. */
size_t stack_size(const Stack *s) { return s->size; }

/** Appends an item, doubling capacity when the array is full. */
DsStatus stack_push(Stack *s, Item x) {
  if (s->size == s->cap) {
    if (s->cap > (size_t)-1 / 2 ||
        s->cap * 2 > (size_t)-1 / sizeof *s->data)
      return DS_NOMEM;
    Item *p = realloc(s->data, s->cap * 2 * sizeof *p);
    if (p == NULL)
      return DS_NOMEM;
    s->data = p;
    s->cap *= 2;
  }
  s->data[s->size++] = x;
  return DS_OK;
}
/** Removes the top item; out may be NULL when the value is not needed. */
DsStatus stack_pop(Stack *s, Item *out) {
  if (s->size == 0)
    return DS_EMPTY;
  Item x = s->data[--s->size];
  if (out != NULL)
    *out = x;
  return DS_OK;
}
/** Copies the top item without changing the stack. */
DsStatus stack_top(const Stack *s, Item *out) {
  if (s->size == 0)
    return DS_EMPTY;
  *out = s->data[s->size - 1];
  return DS_OK;
}
