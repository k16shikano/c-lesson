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
        ch = compile_exec_array(ch, &token);
        main_stack = stack_push(&main_stack, token);
        break;
      case CLOSE_CURLY:
        break;
      case EXECUTABLE_NAME:
        if (strcmp(token.u.name,"add") == 0) {
          main_stack = add_op(main_stack);
        } else if (strcmp(token.u.name,"sub") == 0) {
          main_stack = sub_op(main_stack);
        } else if (strcmp(token.u.name,"mul") == 0) {
          main_stack = mul_op(main_stack);
        } else if (strcmp(token.u.name,"div") == 0) {
          main_stack = div_op(main_stack);
        } else if (strcmp(token.u.name,"eq") == 0) {
          main_stack = eq_op(main_stack);
        } else if (strcmp(token.u.name,"neq") == 0) {
          main_stack = neq_op(main_stack);
        } else if (strcmp(token.u.name,"gt") == 0) {
          main_stack = gt_op(main_stack);
        } else if (strcmp(token.u.name,"ge") == 0) {
          main_stack = ge_op(main_stack);
        } else if (strcmp(token.u.name,"lt") == 0) {
          main_stack = lt_op(main_stack);
        } else if (strcmp(token.u.name,"le") == 0) {
          main_stack = le_op(main_stack);
        } else if (strcmp(token.u.name,"def") == 0) {
          main_stack = def_op(main_stack);
        } else if (strcmp(token.u.name,"pop") == 0) {
          main_stack = pop_op(main_stack);
        } else if (strcmp(token.u.name,"exch") == 0) {
          main_stack = exch_op(main_stack);
        } else if (strcmp(token.u.name,"dup") == 0) {
          main_stack = dup_op(main_stack);
        } else if (strcmp(token.u.name,"index") == 0) {
          main_stack = index_op(main_stack); 
        } else if (strcmp(token.u.name,"roll") == 0) {
          main_stack = roll_op(main_stack);
        } else if (strcmp(token.u.name,"exec") == 0) {
          main_stack = exec_op(main_stack); 
        } else if (strcmp(token.u.name,"if") == 0) {
          main_stack = if_op(main_stack); 
        } else if (strcmp(token.u.name,"ifelse") == 0) {
          main_stack = ifelse_op(main_stack); 
        } else if (strcmp(token.u.name,"while") == 0) {
          main_stack = while_op(main_stack);
        } else {
          if (dict_get(token.u.name, &temp_elem) > 0) {
            if (temp_elem.head.ltype == ELEMENT_C_FUNC) {
              main_stack = temp_elem.head.u.cfunc(main_stack);
            } else if (temp_elem.head.ltype == BYTE_CODES) {
              main_stack = eval_exec_array(main_stack, temp_elem.head.u.byte_codes);
            } else {
              main_stack = stack_push(&main_stack, temp_elem.head);
              //              printf("%s\n", token.u.name);
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

#if 0

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

static void test_eval_num_op(char* input, int expect) {
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
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  result = stack_pop(&result, &token);

  assert(token.u.number == expect);
}

static void test_pop_op() {
  char *input = "1 2 pop";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);

  assert(token.u.number == 1);
}

static void test_exch_op() {
  char *input = "1 2 exch";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);

  assert(token.u.number == 1);
}

static void test_dup_op() {
  char *input = "1 2 dup";
  int expect = 2;
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  result = stack_pop(&result, &token);

  assert(token.u.number == expect);
}

static void test_index_op() {
  char *input = "0 1 2 3 4 3 index";
  int expect = 1;
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == expect);

  result = stack_pop(&result, &token);
  assert(token.u.number == 4);
}

static void test_roll_op() {
  char *input = "1 2 3 4 5 6 7 5 2 roll";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 5);

  result = stack_pop(&result, &token);
  assert(token.u.number == 4);

  result = stack_pop(&result, &token);
  assert(token.u.number == 3);

  result = stack_pop(&result, &token);
  assert(token.u.number == 7);
}

static void test_exec_op() {
  char *input = "{1 2 add}";
  char *input2 = "{21 2 mul} exec";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.ltype == BYTE_CODES);

  cl_getc_set_src(input2);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 42);
}

static void test_if_op() {
  char *input = "0 {1 0 gt} {42 2 sub} if";
  char *input2 = "5 {1 0 lt} {42 2 sub} if";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 40);

  cl_getc_set_src(input2);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 5);
}

static void test_ifelse_op() {
  char *input = "0 {1 0 gt} {42 2 mul} {42 2 sub} ifelse";
  char *input2 = "5 0 {42 2 mul} {42 2 sub} ifelse";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 84);

  cl_getc_set_src(input2);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 40);
}

static void test_while_op() {
  char *input = "/factorial {dup\n%123\n%456\n {dup 1 gt} {1 sub exch 1 index mul exch} while pop} def\n    10 factorial";
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  
  cl_getc_set_src(input);
  
  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  assert(token.u.number == 3628800);
}

void unit_tests() {
  test_eval_num_one();
  test_eval_num_two();
  test_eval_def();
  test_eval_call_def();
  test_eval_num_op("1 2 3 add add 4 5 6 7 8 9 add add add add add add", 45);
  test_eval_num_op("42 2 sub", 40);
  test_eval_num_op("21 2 mul", 42);
  test_eval_num_op("42 20 div", 2);
  test_eval_num_op("1 1 eq", 1);
  test_eval_num_op("1 0 eq", 0);
  test_eval_num_op("1 0 neq", 1);
  test_eval_num_op("1 0 gt", 1);
  test_eval_num_op("0 1 gt", 0);
  test_eval_num_op("1 1 ge", 1);
  test_eval_num_op("1 0 lt", 0);
  test_eval_num_op("0 1 lt", 1);
  test_eval_num_op("1 1 le", 1);

  test_pop_op();
  test_exch_op();
  test_dup_op();
  test_index_op();
  test_roll_op();

  test_exec_op();
  test_if_op();
  test_ifelse_op();
  test_while_op();
}


int main() {
  //  register_primitives();

  unit_tests();
  return 1;
}
#endif
