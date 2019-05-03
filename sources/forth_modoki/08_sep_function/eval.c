#include "clesson.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct Stack* eval(struct Stack* init_stack) {
  int ch = EOF;
  struct Token token = {UNKNOWN,{0}};
  struct Stack* main_stack;
  struct Stack temp_elem = {0};
  stack_init(&main_stack);

  do {
    ch = parse_one(ch, &token);
    if (token.ltype != UNKNOWN) {
      switch (token.ltype) {
      case NUMBER:
        main_stack = stack_push(&main_stack, token);
        break;
      case SPACE:
        break;  
      case OPEN_CURLY:
        break;
      case CLOSE_CURLY:
        break;
      case EXECUTABLE_NAME:
        if (strcmp(token.u.name,"add") == 0) {
          main_stack = add_op(main_stack);
        } else if (strcmp(token.u.name,"def") == 0) {
          main_stack = def_op(main_stack);
        } else {
          if (dict_get(token.u.name, &temp_elem) > 0) {
            if (temp_elem.head.ltype == ELEMENT_C_FUNC) {
              main_stack = temp_elem.head.u.cfunc(main_stack);
            } else {
              main_stack = stack_push(&main_stack, temp_elem.head);
            }
          }
        }
        break;
      case LITERAL_NAME:
        main_stack = stack_push(&main_stack, token);
        break;
        
      default:
        break;
      }
    }    
  } while (ch != EOF);
  return main_stack;
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;
    struct Stack init_stack = {0};
    struct Stack* result = malloc(sizeof(struct Stack));
    struct Token token = {UNKNOWN, {0}};

    cl_getc_set_src(input);

    result = eval(&init_stack);
    result = stack_pop(&result, &token);
    int actual = token.u.number;

    assert(expect == actual);
}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;
    struct Stack init_stack = {0};
    struct Stack* result = malloc(sizeof(struct Stack));
    struct Token token = {UNKNOWN, {0}};
    
    cl_getc_set_src(input);

    result = eval(&init_stack);

    result = stack_pop(&result, &token);
    int actual1 = token.u.number;
    
    result = stack_pop(&result, &token);
    int actual2 = token.u.number;

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}

static void test_eval_num_add() {
    char *input = "1 2 3 add add 4 5 6 7 8 9 add add add add add add";
    int expect = 45;
    struct Stack init_stack = {0};
    struct Stack* result = malloc(sizeof(struct Stack));
    struct Token token = {UNKNOWN, {0}};

    cl_getc_set_src(input);

    result = eval(&init_stack);
    result = stack_pop(&result, &token);

    int actual = token.u.number;
    assert(expect == actual);
}

static void test_eval_def() {
  char *input = "/abc 12 def";
  int expect = 12;
  struct Stack init_stack = {0};
  struct Stack temp_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  dict_get("abc", &temp_stack);

  assert(temp_stack.head.u.number == expect);
}

static void test_eval_call_def() {
  char *input = "/abc 12 def abc abc";
  int expect = 12;
  struct Stack init_stack = {0};
  struct Stack temp_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  result = stack_pop(&result, &token);

  assert(token.u.number == expect);
}

void unit_tests() {
  test_eval_num_one();
  test_eval_num_two();
  test_eval_num_add();
  test_eval_def();
  test_eval_call_def();
}

int main() {
  register_primitives();

  unit_tests();
  return 1;
}
