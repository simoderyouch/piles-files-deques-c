# TD/TP 5 — Piles, files et deques

Correction en langage C11 des exercices sur les structures de données linéaires.
Le projet contient deux implémentations de certaines structures afin de comparer
les représentations contiguës et chaînées.

## Objectifs

- manipuler une pile avec les opérations `push`, `pop` et `top` ;
- vérifier le parenthésage d'une expression avec une pile ;
- implémenter une file FIFO avec un tableau circulaire ou une liste chaînée ;
- implémenter une deque avec un tableau circulaire ;
- transformer et évaluer des expressions arithmétiques en notation infixe ;
- tester les cas limites : structure vide, croissance, erreurs de syntaxe,
  division par zéro et échec d'allocation ;
- comparer les performances des implémentations disponibles.

## Prérequis

- GCC ou un compilateur compatible C11 ;
- `make` pour utiliser les cibles du Makefile ;
- les options de compilation recommandées sont celles utilisées par le projet :
  `-std=c11 -Wall -Wextra -Wpedantic -Werror`.

Les tests principaux sont compilés avec AddressSanitizer et UndefinedBehaviorSanitizer
afin de détecter les fuites mémoire, les dépassements de tampon et les comportements
indéfinis.

## Organisation du projet

| Fichier | Rôle |
| --- | --- |
| `ds_status.h` | Statuts communs (`DS_OK`, `DS_EMPTY`, `DS_NOMEM`, etc.) et type `Item` |
| `stack.h` | Interface abstraite de la pile |
| `stack_array.c` | Pile basée sur un tableau dynamique |
| `stack_list.c` | Pile basée sur une liste simplement chaînée |
| `queue.h` | Interface abstraite de la file |
| `queue_circ.c` | File basée sur un tableau circulaire redimensionnable |
| `queue_list.c` | File basée sur une liste chaînée avec tête et queue |
| `deque.h` | Interface de la deque |
| `deque_circ.c` | Deque basée sur un tableau circulaire |
| `expr.h` / `expr.c` | Analyse, conversion et évaluation d'expressions arithmétiques |
| `util.h` | Fonctions d'affichage utilisées par les exercices |
| `ex01.c` | Simulation des opérations fondamentales d'une pile |
| `ex02.c` | Vérification des délimiteurs équilibrés |
| `ex03.c` | Évaluation autonome d'une expression postfixée |
| `test_stack.c` | Tests communs des deux implémentations de pile |
| `test_queue.c` | Tests communs des deux implémentations de file |
| `test_expr.c` | Tests de l'évaluateur d'expressions |
| `bench.c` | Micro-benchmark pile/file |
| `Makefile` | Commandes de compilation, de test et de benchmark |

## Compiler et exécuter

### Tests disponibles

Les commandes suivantes compilent les tests présents dans le dépôt avec les deux
implémentations lorsqu'elles existent :

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
    test_stack.c stack_array.c -o test_stack_array
./test_stack_array

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
    test_stack.c stack_list.c -o test_stack_list
./test_stack_list

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
    test_queue.c queue_circ.c -o test_queue_circ
./test_queue_circ

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
    test_queue.c queue_list.c -o test_queue_list
./test_queue_list

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
    test_expr.c expr.c stack_array.c queue_circ.c -lm -o test_expr
./test_expr
```

L'option `-lm` est nécessaire pour la fonction `pow` utilisée par l'évaluateur.

### Exercices

Chaque exercice possède son propre `main` :

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror ex01.c stack_array.c -o ex01
./ex01

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror ex02.c stack_array.c -o ex02
./ex02

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror ex03.c -lm -o ex03
./ex03
```

`ex01.c` et `ex02.c` utilisent l'interface de pile. Pour essayer la pile chaînée,
il suffit de remplacer `stack_array.c` par `stack_list.c` dans la commande.

### Makefile

Les cibles prévues sont :

```sh
make        # compilation et exécution des tests
make test   # tests
make bench  # comparaison des performances
make clean  # suppression des exécutables générés
```

À noter : la version actuelle du `Makefile` contient aussi des cibles pour des
fichiers supplémentaires (`test_deque.c`, `deque_dll.c`, `sliding_max.c`,
`round_robin.c`, etc.) qui ne sont pas présents dans cette copie du dépôt.
Ainsi, `make` peut échouer avant d'exécuter les tests disponibles. Les commandes
manuelles ci-dessus correspondent aux fichiers réellement présents.

## API des structures

Les fonctions renvoient un `DsStatus` pour signaler le résultat de l'opération.
Une extraction sur une structure vide renvoie `DS_EMPTY`, sans modifier la valeur
passée par `out`. Le pointeur `out` peut être `NULL` pour `pop` et `dequeue` si la
valeur retirée n'est pas nécessaire.

### Pile

```c
Stack *s = stack_create();
stack_push(s, 10);
stack_push(s, 20);

Item value;
stack_top(s, &value);  /* value == 20, la pile ne change pas */
stack_pop(s, &value);  /* value == 20 */

stack_destroy(s);
```

Les deux implémentations fournissent `push`, `pop`, `top`, `size`,
`is_empty` et `destroy` en temps constant amorti pour `push` avec tableau, et en
temps constant pour toutes les opérations principales avec liste.

### File

La file respecte le principe FIFO : le premier élément ajouté est le premier
retiré.

```c
Queue *q = queue_create();
queue_enqueue(q, 1);
queue_enqueue(q, 2);
queue_dequeue(q, &value);  /* value == 1 */
queue_destroy(q);
```

La version circulaire conserve l'ordre lors du redimensionnement, même lorsque
les éléments sont répartis de part et d'autre de la fin du tableau. La version
chaînée maintient un pointeur `tail` pour réaliser `enqueue` en temps constant.

### Deque

La deque permet l'insertion et le retrait aux deux extrémités :

```c
Deque *d = deque_create();
deque_push_front(d, 2);
deque_push_back(d, 3);
deque_push_front(d, 1);
deque_pop_back(d, &value);  /* value == 3 */
deque_destroy(d);
```

## Évaluateur d'expressions

`expr_eval` réalise les étapes suivantes :

1. vérification des parenthèses `()`, `[]` et `{}` ;
2. analyse lexicale des nombres et opérateurs ;
3. conversion infixe vers postfixe avec l'algorithme de Shunting-yard ;
4. évaluation postfixée avec une pile de nombres réels.

Opérateurs reconnus : `+`, `-`, `*`, `/` et `^`. Les nombres peuvent être
  décimaux ou utiliser un exposant (`1.25`, `2e-3`). Le moins unaire est pris en
  charge, par exemple `-3+4`, `2*-(1+1)` et `2^-1`.

Exemple :

```c
double result;
size_t error_position;
DsStatus status = expr_eval("125.75 + -24.5 * 2.5", &result,
                            &error_position);
```

Les erreurs possibles comprennent notamment `DS_SYNTAX`, `DS_DIVZERO`,
`DS_NOMEM` et `DS_BADARG`. En cas d'erreur de syntaxe, `error_position` indique
la position du premier caractère problématique lorsque l'information est
disponible.

## Complexité et choix d'implémentation

| Structure | Représentation | Opérations principales |
| --- | --- | --- |
| Pile | tableau dynamique | `push` amorti `O(1)`, `pop`/`top` `O(1)` |
| Pile | liste chaînée | `push`/`pop`/`top` `O(1)` |
| File | tableau circulaire | `enqueue` amorti `O(1)`, `dequeue`/`front` `O(1)` |
| File | liste avec tête/queue | `enqueue`/`dequeue`/`front` `O(1)` |
| Deque | tableau circulaire | opérations aux deux extrémités amorties `O(1)` |
| Expressions | piles et files | analyse et conversion en `O(n)` |

Les structures utilisent des types opaques dans les fichiers `.h` : le code
appelant dépend de l'interface et non de la représentation interne. Cela permet
de remplacer une implémentation sans modifier les tests ni les exercices.

## Nettoyage

Les exécutables générés manuellement peuvent être supprimés avec :

```sh
rm -f ex01 ex02 ex03 test_stack_array test_stack_list \
      test_queue_circ test_queue_list test_expr
```

Pour les fichiers générés par le Makefile, utiliser `make clean`.
