/* Exercice 15 - Palindrome avec une deque
 * On insere les caracteres utiles (lettres/chiffres, en minuscules : casse,
 * espaces et ponctuation ignores). Tant que size > 1 : on compare front et
 * back, on retire les deux. Si une paire differe -> non palindrome.
 *   kayak : k=k (retire) ; a=a (retire) ; reste "y" seul -> palindrome
 *   abcd  : a != d -> non palindrome
 * Complexite : O(n) temps, O(n) espace pour la deque.
 */
#include <stdio.h>
#include <ctype.h>
#include "deque.h"

static int is_pal(const char *s, int trace)
{
    Deque *d = deque_create();
    int res = 1;
    Item f, b;
    for (const char *p = s; *p != '\0'; p++)
        if (isalnum((unsigned char)*p)) deque_push_back(d, tolower((unsigned char)*p));
    while (deque_size(d) > 1) {
        deque_front(d, &f);
        deque_back(d, &b);
        if (trace) printf("   compare '%c' et '%c'\n", f, b);
        if (f != b) { res = 0; break; }
        deque_pop_front(d, NULL);
        deque_pop_back(d, NULL);
    }
    deque_destroy(d);
    return res;
}

int main(void)
{
    const char *tests[] = { "RADAR", "KAYAK", "ALGORITHME", "kayak", "abcd",
                            "A man, a plan, a canal: Panama", "" };
    for (size_t i = 0; i < sizeof tests / sizeof tests[0]; i++) {
        int trace = (i == 3 || i == 4);
        if (trace) printf("trace pour \"%s\" :\n", tests[i]);
        printf("\"%s\" -> %s\n", tests[i], is_pal(tests[i], trace) ? "palindrome" : "non palindrome");
    }
    return 0;
}
