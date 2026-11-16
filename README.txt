Correction TD/TP - Piles, files et deques (C11)

Auteur : Mohamed ED Deryouch — GLSID

Bibliotheque du cours (sections 2 a 5) :
  ds_status.h, stack.h/stack_array.c/stack_list.c, queue.h/queue_circ.c/queue_list.c,
  deque.h/deque_circ.c, util.h (affichage des traces)
Exercices : ex01.c ... ex20.c  (la correction theorique est dans le commentaire en tete de chaque fichier)
TP : expr.h, expr.c, test_expr.c, test_stack.c, test_queue.c, bench.c

make        compile et execute les tests (flags stricts)
make run    execute les 20 exercices
make test   tests du TP (memes tests pour tableau et liste)
make bench  benchmark pile tableau vs liste
make sanitize  tests avec AddressSanitizer et UndefinedBehaviorSanitizer
