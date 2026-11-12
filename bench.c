/* make bench : n push puis n pop (pile). Lier avec stack_array.c ou stack_list.c */
#include <stdio.h>
#include <time.h>
#include "stack.h"

int main(void)
{
    const int n = 5000000;
    Stack *s = stack_create();
    clock_t t0 = clock();
    for (int i = 0; i < n; i++) stack_push(s, i);
    for (int i = 0; i < n; i++) stack_pop(s, NULL);
    printf("%d push + %d pop : %.0f ms\n", n, n, 1000.0 * (double)(clock() - t0) / CLOCKS_PER_SEC);
    stack_destroy(s);
    return 0;
}
