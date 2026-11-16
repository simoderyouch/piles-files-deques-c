#include <assert.h>
#include <stdio.h>
#include "deque.h"

int main(void)
{
    Deque *d = deque_create();
    Item value = 777;
    assert(d != NULL && deque_is_empty(d));
    assert(deque_pop_front(d, &value) == DS_EMPTY && value == 777);
    assert(deque_pop_back(d, &value) == DS_EMPTY && value == 777);
    for (int i = 0; i < 4; i++) assert(deque_push_back(d, i) == DS_OK);
    assert(deque_pop_front(d, &value) == DS_OK && value == 0);
    assert(deque_pop_front(d, &value) == DS_OK && value == 1);
    for (int i = 4; i < 20; i++) assert(deque_push_back(d, i) == DS_OK);
    assert(deque_push_front(d, 1) == DS_OK);
    assert(deque_front(d, &value) == DS_OK && value == 1);
    assert(deque_back(d, &value) == DS_OK && value == 19);
    assert(deque_size(d) == 19);
    for (int i = 19; i >= 1; i--)
        assert(deque_pop_back(d, &value) == DS_OK && value == i);
    assert(deque_is_empty(d));
    assert(deque_push_front(d, 42) == DS_OK);
    assert(deque_pop_front(d, NULL) == DS_OK && deque_is_empty(d));
    deque_destroy(d);
    deque_destroy(NULL);
    puts("test_deque : OK");
    return 0;
}
