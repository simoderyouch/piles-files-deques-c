/* Exercice 20 - Choix de la structure adaptee
 *
 *  Situation                      Structure  Justification / algorithme (complexite)
 *  ------------------------------ ---------- ------------------------------------------------
 *  Annuler / Retablir             2 piles    LIFO : undo depile l'action et l'empile dans redo. O(1)/action
 *  Requetes FIFO                  file       premier arrive, premier servi. enqueue/dequeue O(1)
 *  DFS                            pile       dernier decouvert explore d'abord. O(V+E)
 *  Palindrome                     deque      compare les deux extremites. O(n)
 *  Minimum glissant               deque      deque monotone d'indices. O(n)
 *  Parcours d'arbre par niveaux   file       BFS : on traite les noeuds par profondeur croissante. O(n)
 *  Appels imbriques               pile       cadres empiles a l'appel, depiles au retour. O(1)/appel
 *  Taches modifiables aux 2 bouts deque      ajout/retrait devant et derriere en O(1)
 *
 * Demonstrations ci-dessous : undo/redo (2 piles) et parcours par niveaux (file).
 */
#include <stdio.h>
#include "stack.h"
#include "queue.h"

/* arbre binaire en tableau : fils gauche 2i+1, fils droit 2i+2 */
static void level_order(const int *t, int n)
{
    Queue *q = queue_create();
    Item i;
    queue_enqueue(q, 0);
    printf("niveaux :");
    while (queue_dequeue(q, &i) == DS_OK) {
        printf(" %d", t[i]);
        if (2 * i + 1 < n) queue_enqueue(q, 2 * i + 1);
        if (2 * i + 2 < n) queue_enqueue(q, 2 * i + 2);
    }
    printf("\n");
    queue_destroy(q);
}

int main(void)
{
    Stack *undo = stack_create(), *redo = stack_create();
    Item a;
    for (int act = 1; act <= 3; act++) { stack_push(undo, act); stack_pop(redo, NULL); }
    printf("actions faites : 1 2 3\n");
    stack_pop(undo, &a); stack_push(redo, a); printf("annuler  -> action %d\n", a);
    stack_pop(undo, &a); stack_push(redo, a); printf("annuler  -> action %d\n", a);
    stack_pop(redo, &a); stack_push(undo, a); printf("retablir -> action %d\n", a);
    stack_destroy(undo); stack_destroy(redo);

    int tree[] = {1, 2, 3, 4, 5, 6, 7};
    level_order(tree, 7);
    return 0;
}
