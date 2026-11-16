CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
LDLIBS = -lm
BUILD = .build
EXERCISES = $(patsubst %.c,$(BUILD)/%,$(wildcard ex[0-9][0-9].c))
TESTS = $(addprefix $(BUILD)/,test_stack_array test_stack_list test_queue_circ test_queue_list test_deque test_expr_array test_expr_list)

.PHONY: all test run bench sanitize clean
all: test

$(BUILD):
	mkdir -p $@

$(BUILD)/test_stack_array: test_stack.c stack_array.c stack.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_stack.c stack_array.c -o $@ $(LDLIBS)
$(BUILD)/test_stack_list: test_stack.c stack_list.c stack.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_stack.c stack_list.c -o $@ $(LDLIBS)
$(BUILD)/test_queue_circ: test_queue.c queue_circ.c queue.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_queue.c queue_circ.c -o $@ $(LDLIBS)
$(BUILD)/test_queue_list: test_queue.c queue_list.c queue.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_queue.c queue_list.c -o $@ $(LDLIBS)
$(BUILD)/test_deque: test_deque.c deque_circ.c deque.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_deque.c deque_circ.c -o $@ $(LDLIBS)
$(BUILD)/test_expr_array: test_expr.c expr.c stack_array.c queue_circ.c expr.h stack.h queue.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_expr.c expr.c stack_array.c queue_circ.c -o $@ $(LDLIBS)
$(BUILD)/test_expr_list: test_expr.c expr.c stack_list.c queue_list.c expr.h stack.h queue.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) test_expr.c expr.c stack_list.c queue_list.c -o $@ $(LDLIBS)

test: $(TESTS)
	@set -e; for test in $(TESTS); do ./$$test; done

$(BUILD)/ex%: ex%.c stack_array.c queue_circ.c deque_circ.c stack.h queue.h deque.h ds_status.h util.h | $(BUILD)
	$(CC) $(CFLAGS) $< stack_array.c queue_circ.c deque_circ.c -o $@ $(LDLIBS)

run: $(EXERCISES)
	@set -e; for exercise in $(EXERCISES); do echo "--- $$exercise ---"; ./$$exercise; done

$(BUILD)/bench_array: bench.c stack_array.c queue_circ.c stack.h queue.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) -DIMPL_NAME='"array"' bench.c stack_array.c queue_circ.c -o $@ $(LDLIBS)
$(BUILD)/bench_list: bench.c stack_list.c queue_list.c stack.h queue.h ds_status.h | $(BUILD)
	$(CC) $(CFLAGS) -DIMPL_NAME='"list"' bench.c stack_list.c queue_list.c -o $@ $(LDLIBS)
bench: $(BUILD)/bench_array $(BUILD)/bench_list
	./$(BUILD)/bench_array
	./$(BUILD)/bench_list

sanitize:
	$(MAKE) BUILD=$(BUILD)/sanitize CFLAGS='-std=c11 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1' test

clean:
	rm -rf $(BUILD)
	rm -f t_stack_array t_stack_list t_queue_circ t_queue_list t_deque_circ t_deque_dll t_expr_array t_expr_list t_sliding_circ t_sliding_dll t_rr_circ t_rr_list t_recursion t_stack_fixed t_pal_array t_pal_list t_ep_array t_ep_list bench_array bench_list
