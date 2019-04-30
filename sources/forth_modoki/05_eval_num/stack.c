#include "clesson.h"
#include <stdlib.h>
#include <assert.h>

#define STACK_SIZE 1024

struct Stack* stack_pop(struct Stack* stack, struct Token* out_token) {
  if (stack->depth == 0) {
    out_token->ltype = UNKNOWN;
    return stack;
  }
  out_token->ltype = stack->head->ltype;
  out_token->u = stack->head->u;
  return stack->tail;
}

struct Stack* stack_push(struct Stack* stack, struct Token* token) {
  struct Stack* result;

  if (stack->depth > STACK_SIZE) {
    return stack;
  }
  result = malloc(sizeof(struct Stack));
  result->depth = stack->depth + 1;
  result->head = token;
  result->tail = stack;
  return result;
}

#if 0
static void test_push_pop() {
  struct Stack main_stack = {0};
  main_stack.depth = 0;
  struct Token token1 = {NUMBER, { .number = 42 }};
  struct Token token2 = {EXECUTABLE_NAME, { .name = "add" }};
  struct Token token = {UNKNOWN, {0}};
  struct Stack *result;

  result = stack_push(&main_stack, &token1);
  result = stack_push(result, &token2);

  assert(result->depth == 2);

  result = stack_pop(result, &token);

  assert(result->depth == 1);
  assert(token.ltype == EXECUTABLE_NAME);
  assert(token.u.name == "add");
}

static void test_push() {
  struct Stack main_stack = {0};
  main_stack.depth = 0;
  struct Token token = {UNKNOWN, {0}};
  struct Token token1 = {NUMBER, { .number = 42 }};
  struct Stack* result;

  result = stack_push(&main_stack, &token1);

  assert(result->head->ltype == NUMBER);
  assert(result->head->u.number == 42);
}

static void test_pop() {  
  struct Stack main_stack = {0};
  //main_stack.u.depth = 0; // need or not?
  struct Token token = {UNKNOWN, {0}};
  struct Stack* result; 

  result = stack_pop(&main_stack, &token);

  assert(token.ltype == UNKNOWN);
  assert(result->depth == 0);
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
