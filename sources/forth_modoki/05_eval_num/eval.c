#include "clesson.h"
#include <stdlib.h>
#include <assert.h>

struct Stack* eval(struct Stack* init_stack) {
  int ch = EOF;
  struct Token token = {UNKNOWN,{0}};
  struct Stack* result;
  result = init_stack;

  do {
    ch = parse_one(ch, &token);
    if(token.ltype != UNKNOWN) {
      switch(token.ltype) {
      case NUMBER:
        printf("num: %d\n", token.u.number);
        result = stack_push(result, &token);
        break;
      case SPACE:
        printf("space!\n");
        break;
      case OPEN_CURLY:
        printf("Open curly brace '%c'\n", token.u.onechar);
        break;
      case CLOSE_CURLY:
        printf("Close curly brace '%c'\n", token.u.onechar);
        break;
      case EXECUTABLE_NAME:
        printf("EXECUTABLE_NAME: %s\n", token.u.name);
        break;
      case LITERAL_NAME:
        printf("LITERAL_NAME: %s\n", token.u.name);
        break;
        
      default:
        printf("Unknown type %d\n", token.ltype);
        break;
      }
    }
  }while(ch != EOF);
  return result;
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;
    struct Stack init_stack = {0};
    struct Stack* result;
    struct Token token = {UNKNOWN, {0}};

    cl_getc_set_src(input);

    result = eval(&init_stack);

    stack_pop(result, &token);
    int actual = token.u.number;

    assert(expect == actual);

}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;
    struct Stack init_stack = {0};
    struct Stack* result;
    struct Token token = {UNKNOWN, {0}};

    cl_getc_set_src(input);

    result = eval(&init_stack);

    result = stack_pop(result, &token);
    int actual1 = token.u.number;
    result = stack_pop(result, &token);
    printf("%d\n",token.u.number);
    int actual2 = token.u.number;

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}


static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;
    struct Stack init_stack = {0};
    struct Stack* result;
    struct Token token = {UNKNOWN, {0}};

    cl_getc_set_src(input);

    result = eval(&init_stack);

    /* TODO: write code to pop stack top element */
    int actual = 0;
    assert(expect == actual);
}


int main() {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();

    return 1;
}
