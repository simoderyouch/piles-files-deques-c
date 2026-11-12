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

Stack *stack_create(void) {
  Stack *s = malloc(sizeof *s);
  if (s == NULL)
    return NULL;
  s->head = NULL;
  s->size = 0;
  return s;
}
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
bool stack_is_empty(const Stack *s) { return s->head == NULL; }
size_t stack_size(const Stack *s) { return s->size; }
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
DsStatus stack_top(const Stack *s, Item *out) {
  if (s->head == NULL)
    return DS_EMPTY;
  *out = s->head->value;
  return DS_OK;
}
