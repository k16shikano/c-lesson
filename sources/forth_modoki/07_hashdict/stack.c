#include "clesson.h"
#include <stdlib.h>
#include <assert.h>

#define STACK_SIZE 1024

int stack_init(struct Stack** stack) {
  (*stack) = malloc(sizeof(struct Stack) * STACK_SIZE);
  (*stack)->depth = 0;
  return 1;
}

struct Stack* stack_pop(struct Stack** stack, struct Token* out_token) {
  struct Stack* old;

  if (*stack == NULL) {
    printf("EXIT: Stuck Underflow.\n");
    exit(1);
  }

  *out_token = (*stack)->head;
  old = *stack;
  *stack = (*stack)->tail;

  free(old);

  return *stack;
}

struct Stack* stack_push(struct Stack** stack, struct Token token) {
  struct Stack* new = NULL;
  new = malloc(sizeof(struct Stack));
  new->head = token;
  new->tail = *stack;
  new->depth = (*stack)->depth + 1;

  *stack = new;

  return new;
}

#if 0
static void test_push_pop() {
  struct Stack* main_stack = NULL;
  main_stack = malloc(sizeof(struct Stack));
  struct Token token = {NUMBER, { .number = 0 }};
  int d = 0;

  stack_init(&main_stack);
  
  while (main_stack->depth < 10) {
    d++;
    token.u.number = d;
    main_stack = stack_push(&main_stack, token);
    printf("%d\n",main_stack->head.u.number);
    printf("%d\n",main_stack->tail->head.u.number);
  }

  main_stack = stack_pop(&main_stack, &token);
  assert(token.u.number == 10);
  assert(main_stack->depth == 9);

  main_stack = stack_pop(&main_stack, &token);
  assert(token.u.number == 9);
  assert(main_stack->depth == 8);
}

static void test_push() {
  struct Stack* main_stack = NULL;
  struct Token token1 = {NUMBER, { .number = 42 }};
  
  stack_init(&main_stack);
  main_stack = stack_push(&main_stack, token1);

  assert(main_stack->depth == 1);
  assert(main_stack->head.ltype == NUMBER);
  assert(main_stack->head.u.number == 42);
}

static void test_pop() {  
  struct Stack* main_stack = NULL;
  struct Token token = {UNKNOWN,{0}};

  stack_init(&main_stack);
  main_stack = stack_pop(&main_stack, &token);

  // initial stal is {0}
  assert(token.ltype == 0);
}

static void unit_tests() {
  test_pop();
  test_push();
  test_push_pop();
}

int main() {
  unit_tests();
  return 1;
}
#endif
