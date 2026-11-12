/* Meme test lie aux deux implementations (stack_array.c et stack_list.c) */
#include <assert.h>
#include <stdio.h>
#include "stack.h"

/** Exercises both stack implementations through the common public interface. */
int main(void)
{
    Stack *s = stack_create();
    Item x = 777;
    assert(s != NULL && stack_is_empty(s) && stack_size(s) == 0);
    assert(stack_pop(s, &x) == DS_EMPTY && x == 777);      /* pop a vide, *out intact */
    assert(stack_top(s, &x) == DS_EMPTY && x == 777);
    assert(stack_push(s, 1) == DS_OK && stack_push(s, 2) == DS_OK);
    assert(stack_top(s, &x) == DS_OK && x == 2 && stack_size(s) == 2);   /* top ne retire rien */
    assert(stack_pop(s, &x) == DS_OK && x == 2);
    assert(stack_pop(s, NULL) == DS_OK && stack_is_empty(s));
    for (int i = 0; i < 1000; i++) assert(stack_push(s, i) == DS_OK);    /* croissance */
    assert(stack_size(s) == 1000);
    for (int i = 999; i >= 0; i--) { assert(stack_pop(s, &x) == DS_OK && x == i); }
    assert(stack_is_empty(s));
    stack_push(s, 5);
    stack_destroy(s);               /* avec element restant : pas de fuite (ASan) */
    stack_destroy(NULL);
    printf("test_stack : OK\n");
    return 0;
}
