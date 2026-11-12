/* Exercice 2 - Delimiteurs equilibres et localisation des erreurs
 *
 * On empile la POSITION de chaque ouvrant ( ( [ { ).
 * Pour un fermant :
 *   - pile vide            -> fermeture inattendue (position du fermant)
 *   - mauvais ouvrant      -> mauvais appariement (position du fermant)
 *   - fin de chaine, pile non vide -> ouverture non refermee (position de
 * l'ouvrant) Complexite : chaque caractere est lu une fois, push/pop en O(1) ->
 * O(n) temps, O(n) espace dans le pire cas (que des ouvrants).
 */




#include "stack.h"
#include <stdio.h>

typedef enum { B_OK, B_UNEXPECTED_CLOSE, B_MISMATCH, B_UNCLOSED, B_NOMEM } BErr;

static int is_open(char c) { return c == '(' || c == '[' || c == '{'; }
static int is_close(char c) { return c == ')' || c == ']' || c == '}'; }

static int match(char o, char c) {
  return (o == '(' && c == ')') || (o == '[' && c == ']') ||
         (o == '{' && c == '}');
}

static BErr check(const char *s, size_t *pos, size_t *open_pos) {
  Stack *st = stack_create();
  BErr res = B_OK;
  size_t i;
  if (st == NULL)
    return B_NOMEM;
  for (i = 0; s[i] != '\0' && res == B_OK; i++) {
    if (is_open(s[i])) {
      if (stack_push(st, (Item)i) != DS_OK) {
        res = B_NOMEM;
        break;
      }
    } else if (is_close(s[i])) {
      Item o;
      if (stack_pop(st, &o) == DS_EMPTY) {
        res = B_UNEXPECTED_CLOSE;
        *pos = i;
      } else if (!match(s[o], s[i])) {
        res = B_MISMATCH;
        *pos = i;
        *open_pos = (size_t)o;
      }
    }
  }
  if (res == B_OK && !stack_is_empty(st)) {
    Item o;
    stack_top(st, &o);
    res = B_UNCLOSED;
    *pos = (size_t)o;
  }
  stack_destroy(st);
  return res;
}

int main(void) {
  const char *tests[] = {"{[a+b]*(c-d)}", "([a+b)]", "((a+b)*[c-d])",
                         "{[()]}",        "([)]",    "((x), x)",
                         "{(}",           "a+b)",    "((a"};

  for (size_t k = 0; k < sizeof tests / sizeof tests[0]; k++) {

    size_t pos = 0, op = 0;
    BErr e = check(tests[k], &pos, &op);
    printf("%-16s : ", tests[k]);
    switch (e) {
    case B_OK:
      printf("equilibre\n");
      break;
    case B_UNEXPECTED_CLOSE:
      printf("fermeture inattendue '%c' en position %zu\n", tests[k][pos], pos);
      break;
    case B_MISMATCH:
      printf("mauvais appariement : '%c' (pos %zu) ferme par '%c' (pos %zu)\n",
             tests[k][op], op, tests[k][pos], pos);
      break;
    case B_UNCLOSED:
      printf("ouverture '%c' en position %zu jamais refermee\n", tests[k][pos],
             pos);
      break;
    case B_NOMEM:
      printf("memoire insuffisante\n");
      break;
    }
  }
  return 0;
}
