#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stddef.h>
#include "ds_status.h"
typedef struct Stack Stack;

/** Creates an empty stack.
 *  @return A new stack, or NULL when allocation fails.
 */
Stack *stack_create(void);

/** Releases the stack and all nodes/storage owned by it. NULL is accepted. */
void stack_destroy(Stack *s);

/** @return true when the stack contains no item. */
bool stack_is_empty(const Stack *s);

/** @return The number of items currently stored in the stack. */
size_t stack_size(const Stack *s);

/** Pushes an item onto the top of the stack.
 *  @return DS_OK on success, or DS_NOMEM if storage cannot be expanded.
 */
DsStatus stack_push(Stack *s, Item x);

/** Removes the top item and optionally writes it to out.
 *  @return DS_OK on success, or DS_EMPTY when the stack is empty.
 */
DsStatus stack_pop(Stack *s, Item *out);

/** Reads the top item without removing it.
 *  @return DS_OK on success, or DS_EMPTY when the stack is empty.
 */
DsStatus stack_top(const Stack *s, Item *out);
#endif
