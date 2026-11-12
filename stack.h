#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stddef.h>
#include "ds_status.h"
typedef struct Stack Stack;
Stack *stack_create(void);
void stack_destroy(Stack *s);
bool stack_is_empty(const Stack *s);
size_t stack_size(const Stack *s);
DsStatus stack_push(Stack *s, Item x);
DsStatus stack_pop(Stack *s, Item *out);
DsStatus stack_top(const Stack *s, Item *out);
#endif
