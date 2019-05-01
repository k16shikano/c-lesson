#include "clesson.h"
#include <stdlib.h>
#include <assert.h>
#include<string.h>

struct Stack* eval(struct Stack* init_stack) {
  int ch = EOF;
  struct Token token = {UNKNOWN,{0}};
  struct Stack* main_stack;
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
          main_stack = stack_pop(&main_stack, &token);
          int var1 = token.u.number;
          main_stack = stack_pop(&main_stack, &token);
          int var2 = token.u.number;
          int val = var1 + var2;
          token.u.number = val;
          main_stack = stack_push(&main_stack, token);
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


int main() {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();

    return 1;
}
