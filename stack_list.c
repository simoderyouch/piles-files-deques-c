/* Pile : liste simplement chainee (cours 2.3 B) */
#include "stack.h"
#include <stdlib.h>

typedef struct Node {
  Item value;
  struct Node *next;
} Node;
struct Stack {
  Node *head;
  size_t size;
};

/** Allocates an empty stack whose first node will be the list head. */
Stack *stack_create(void) {
  Stack *s = malloc(sizeof *s);
  if (s == NULL)
    return NULL;
  s->head = NULL;
  s->size = 0;
  return s;
}
/** Walks through and frees every node before freeing the stack. */
void stack_destroy(Stack *s) {
  if (s == NULL)
    return;
  Node *cur = s->head;
  while (cur != NULL) {
    Node *next = cur->next;
    free(cur);
    cur = next;
  }
  free(s);
}
/** Tests whether the linked list has no head node. */
bool stack_is_empty(const Stack *s) { return s->head == NULL; }
/** Returns the number of nodes in the stack. */
size_t stack_size(const Stack *s) { return s->size; }
/** Creates a node and links it before the current head. */
DsStatus stack_push(Stack *s, Item x) {
  Node *n = malloc(sizeof *n);
  if (n == NULL)
    return DS_NOMEM;
  n->value = x;
  n->next = s->head;
  s->head = n;
  s->size++;
  return DS_OK;
}
/** Unlinks and frees the head node; out may be NULL. */
DsStatus stack_pop(Stack *s, Item *out) {
  if (s->head == NULL)
    return DS_EMPTY;
  Node *n = s->head;
  if (out != NULL)
    *out = n->value;
  s->head = n->next;
  free(n);
  s->size--;
  return DS_OK;
}
/** Reads the head value without unlinking its node. */
DsStatus stack_top(const Stack *s, Item *out) {
  if (s->head == NULL)
    return DS_EMPTY;
  *out = s->head->value;
  return DS_OK;
}
