CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g
LIB = stack_array.c queue_circ.c deque_circ.c
EX = $(basename $(wildcard ex[0-9][0-9].c))

all: $(addprefix build/,$(EX)) tp

build:
	mkdir -p build

build/ex%: ex%.c $(LIB) | build
	$(CC) $(CFLAGS) $< $(LIB) -o $@

# ---- TP : memes tests pour les deux implementations ----
tp: | build
	$(CC) $(CFLAGS) test_stack.c stack_array.c -o build/t_stack_array
	$(CC) $(CFLAGS) test_stack.c stack_list.c  -o build/t_stack_list
	$(CC) $(CFLAGS) test_queue.c queue_circ.c  -o build/t_queue_circ
	$(CC) $(CFLAGS) test_queue.c queue_list.c  -o build/t_queue_list
	$(CC) $(CFLAGS) test_expr.c expr.c stack_array.c queue_circ.c -lm -o build/t_expr
	$(CC) -std=c11 -O2 -Wall -Wextra bench.c stack_array.c -o build/bench_stack_array
	$(CC) -std=c11 -O2 -Wall -Wextra bench.c stack_list.c  -o build/bench_stack_list

run: all
	@for e in $(EX); do echo "================ $$e"; ./build/$$e || exit 1; done

test: tp
	./build/t_stack_array && ./build/t_stack_list && ./build/t_queue_circ && ./build/t_queue_list && ./build/t_expr

bench: tp
	./build/bench_stack_array && ./build/bench_stack_list

clean:
	rm -rf build
