/* Exercice 8 - Specification axiomatique du TAD File
 *
 * Signatures :
 *   creer    : -> File
 *   enfiler  : File x Element -> File
 *   defiler  : File -> File          pre : non estVide(f)
 *   tete     : File -> Element       pre : non estVide(f)
 *   estVide  : File -> Booleen
 *
 * Axiomes :
 *   (A1) estVide(creer()) = vrai
 *   (A2) estVide(enfiler(f,x)) = faux
 *   (A3) tete(enfiler(f,x)) = si estVide(f) alors x sinon tete(f)
 *   (A4) defiler(enfiler(f,x)) = si estVide(f) alors creer()
 *                                sinon enfiler(defiler(f), x)
 *
 * Demonstration : tete(enfiler(enfiler(creer(), a), b)) = a
 *   Soit f = enfiler(creer(), a).
 *   tete(enfiler(f, b)) = si estVide(f) alors b sinon tete(f)        par (A3)
 *   estVide(f) = estVide(enfiler(creer(), a)) = faux                 par (A2)
 *   donc tete(enfiler(f, b)) = tete(f) = tete(enfiler(creer(), a))
 *   = si estVide(creer()) alors a sinon tete(creer())                par (A3)
 *   = a                                                              par (A1)   CQFD
 *
 * Specification abstraite (fonctions pures, pas de memoire, pas d'erreur
 * d'allocation) / representation concrete (tableau circulaire ou liste,
 * modification en place, codes DS_EMPTY / DS_NOMEM).
 * Ci-dessous : verification des axiomes sur l'implementation C.
 */
#include <stdio.h>
#include <assert.h>
#include "queue.h"

int main(void)
{
    Item t;
    Queue *q = queue_create();
    assert(queue_is_empty(q));                                   /* A1 */
    queue_enqueue(q, 1);
    assert(!queue_is_empty(q));                                  /* A2 */
    queue_front(q, &t); assert(t == 1);                          /* A3, f vide */
    queue_enqueue(q, 2);
    queue_front(q, &t); assert(t == 1);                          /* A3, f non vide : theoreme */
    queue_enqueue(q, 3);
    queue_dequeue(q, NULL);                                      /* A4 : l'ancien disparait... */
    queue_front(q, &t); assert(t == 2);
    queue_dequeue(q, &t); assert(t == 2);                        /* ...et l'ordre est conserve */
    queue_dequeue(q, &t); assert(t == 3);
    assert(queue_is_empty(q));
    queue_destroy(q);
    printf("axiomes A1..A4 et theoreme verifies sur l'implementation\n");
    return 0;
}
