/* Exercice 1 - Simulation et operations fondamentales d'une pile
 *
 * Sequence : Push(5) Push(8) Push(3) Pop() Push(12) Pop() Push(7) Push(4) Pop()
 *
 * Trace (fond -> sommet) :
 *   Push(5)  : 5
 *   Push(8)  : 5 8
 *   Push(3)  : 5 8 3
 *   Pop()    : renvoie 3   -> 5 8
 *   Push(12) : 5 8 12
 *   Pop()    : renvoie 12  -> 5 8
 *   Push(7)  : 5 8 7
 *   Push(4)  : 5 8 7 4
 *   Pop()    : renvoie 4   -> 5 8 7
 * Valeurs depilees : 3, 12, 4.  Pile finale : 5 8 7, sommet = 7.
 *
 * push, pop, top, isEmpty, destroy : voir stack_array.c (cours 2.4).
 */
#include "stack.h"
#include "util.h"
#include <stdio.h>

int main(void) {
  Stack *s = stack_create();
  if (s == NULL) {
    fprintf(stderr, "Erreur : allocation de la pile impossible.\n");
    return 1;
  }
  Item x;
  if (stack_push(s, 5) != DS_OK) { stack_destroy(s); return 1; }
  stack_print("Push(5) ", s, 0);
  if (stack_push(s, 8) != DS_OK) { stack_destroy(s); return 1; }
  stack_print("Push(8) ", s, 0);
  if (stack_push(s, 3) != DS_OK) { stack_destroy(s); return 1; }
  stack_print("Push(3) ", s, 0);
  if (stack_pop(s, &x) != DS_OK) { stack_destroy(s); return 1; }
  printf("Pop -> %d\n", x);
  stack_print("        ", s, 0);
  if (stack_push(s, 12) != DS_OK) { stack_destroy(s); return 1; }
  stack_print("Push(12)", s, 0);
  if (stack_pop(s, &x) != DS_OK) { stack_destroy(s); return 1; }
  printf("Pop -> %d\n", x);
  stack_print("        ", s, 0);
  if (stack_push(s, 7) != DS_OK) { stack_destroy(s); return 1; }
  stack_print("Push(7) ", s, 0);
  if (stack_push(s, 4) != DS_OK) { stack_destroy(s); return 1; }
  stack_print("Push(4) ", s, 0);
  if (stack_pop(s, &x) != DS_OK) { stack_destroy(s); return 1; }
  printf("Pop -> %d\n", x);
  stack_print("        ", s, 0);

  if (stack_top(s, &x) != DS_OK) { stack_destroy(s); return 1; }
  printf("Sommet final = %d, taille = %zu\n", x, stack_size(s));

  /* Test : depilement sur pile vide */
  while (stack_pop(s, NULL) == DS_OK) {
  }
  printf("pile vide ? %s\n", stack_is_empty(s) ? "oui" : "non");
  DsStatus st = stack_pop(s, &x);
  printf("pop sur pile vide -> %s\n",
         st == DS_EMPTY ? "DS_EMPTY (erreur detectee)" : "?!");
  st = stack_top(s, &x);
  printf("top sur pile vide -> %s\n",
         st == DS_EMPTY ? "DS_EMPTY (erreur detectee)" : "?!");
  stack_destroy(s);
  return 0;
}
