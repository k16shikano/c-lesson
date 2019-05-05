#include "clesson.h"
#include <stdlib.h>

void type_error() {
  printf("type error\n");
  exit(0);
}

struct Stack* arith_op(struct Stack* main_stack, int (*op)(int a, int b)) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token);
  if (token.ltype != NUMBER) { type_error(); }
  int var1 = token.u.number;

  main_stack = stack_pop(&main_stack, &token);
  if (token.ltype != NUMBER) { type_error(); }
  int var2 = token.u.number;

  int val = op(var1, var2);
  token.u.number = val;
  main_stack = stack_push(&main_stack, token);
  return main_stack;
}
int my_sub(int v1, int v2) {return v2 - v1;}
int my_add(int v1, int v2) {return v2 + v1;}
int my_mul(int v1, int v2) {return v2 * v1;}
int my_div(int v1, int v2) {return v2 / v1;}
int my_eq(int v1, int v2)  {return v2 == v1;}
int my_neq(int v1, int v2) {return v2 != v1;}
int my_gt(int v1, int v2)  {return v2 > v1;}
int my_ge(int v1, int v2)  {return v2 >= v1;}
int my_lt(int v1, int v2)  {return v2 < v1;}
int my_le(int v1, int v2)  {return v2 <= v1;}
struct Stack* sub_op(struct Stack* main_stack) {arith_op(main_stack, my_sub);}
struct Stack* add_op(struct Stack* main_stack) {arith_op(main_stack, my_add);}
struct Stack* mul_op(struct Stack* main_stack) {arith_op(main_stack, my_mul);}
struct Stack* div_op(struct Stack* main_stack) {arith_op(main_stack, my_div);}
struct Stack* eq_op(struct Stack* main_stack)  {arith_op(main_stack, my_eq);}
struct Stack* neq_op(struct Stack* main_stack) {arith_op(main_stack, my_neq);}
struct Stack* gt_op(struct Stack* main_stack)  {arith_op(main_stack, my_gt);}
struct Stack* ge_op(struct Stack* main_stack)  {arith_op(main_stack, my_ge);}
struct Stack* lt_op(struct Stack* main_stack)  {arith_op(main_stack, my_lt);}
struct Stack* le_op(struct Stack* main_stack)  {arith_op(main_stack, my_le);}

struct Stack* pop_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token);
  return main_stack;
}

struct Stack* exch_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token);
  struct Token t1 = token;
  main_stack = stack_pop(&main_stack, &token);
  struct Token t2 = token; 

  main_stack = stack_push(&main_stack, t1);
  main_stack = stack_push(&main_stack, t2);
  return main_stack;
}

struct Stack* dup_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token);
  struct Token t1 = token;

  main_stack = stack_push(&main_stack, t1);
  main_stack = stack_push(&main_stack, t1);
  return main_stack;
}

struct Stack* index_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token);

  if (token.ltype != NUMBER) { type_error(); }

  struct Token* t[token.u.number];
  
  for (int i = 0; i <= token.u.number; i++) {
    t[i] = malloc(sizeof(struct Token));
    main_stack = stack_pop(&main_stack, t[i]);
  }
  for (int j = token.u.number; j >= 0; j--) {
    main_stack = stack_push(&main_stack, *t[j]);
  }
  main_stack = stack_push(&main_stack, *t[token.u.number]);
  return main_stack;
}

struct Stack* roll_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  
  main_stack = stack_pop(&main_stack, &token);
  if (token.ltype != NUMBER) { type_error(); }
  int roll_times = token.u.number;

  main_stack = stack_pop(&main_stack, &token);
  if (token.ltype != NUMBER) { type_error(); }
  struct Token* t[token.u.number];

  for (int k = 0; k < roll_times; k++) {
    for (int i = 0; i < token.u.number; i++) {
      t[i] = malloc(sizeof(struct Token));
      main_stack = stack_pop(&main_stack, t[i]);
    }
    main_stack = stack_push(&main_stack, *t[0]);
    for (int j = token.u.number-1; j > 0; j--) {
      main_stack = stack_push(&main_stack, *t[j]);
    }
  }
  return main_stack;
}

struct Stack* exec_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token); 

  if (token.ltype != BYTE_CODES) { type_error(); }
  main_stack = eval_exec_array(main_stack, token.u.byte_codes);

  return main_stack;
}

struct Stack* if_op(struct Stack* main_stack) {
  struct Token token1 = {UNKNOWN,{0}}; 
  struct Token token2 = {UNKNOWN,{0}};

  main_stack = stack_pop(&main_stack, &token1);
  if (token1.ltype != BYTE_CODES) { type_error(); }

  int test;
  main_stack = stack_pop(&main_stack, &token2);
  if (token2.ltype == NUMBER) {
    test = token2.u.number;
  } else if (token2.ltype == BYTE_CODES) {
    struct Token token = {UNKNOWN,{0}};
    struct Stack* local_stack = malloc(sizeof(struct Stack));
    local_stack = eval_exec_array(local_stack, token2.u.byte_codes);
    stack_pop(&local_stack, &token);
    test = token.u.number;
  } else {
    type_error();
  }

  if (test) {
    main_stack = eval_exec_array(main_stack, token1.u.byte_codes);
  }

  return main_stack;
}

struct Stack* ifelse_op(struct Stack* main_stack) { 
  struct Token false_token = {UNKNOWN,{0}};
  struct Token true_token = {UNKNOWN,{0}};

  main_stack = stack_pop(&main_stack, &false_token);
  if (false_token.ltype != BYTE_CODES) { type_error(); }
  main_stack = stack_pop(&main_stack, &true_token);
  if (true_token.ltype != BYTE_CODES) { type_error(); }

  struct Token test_token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &test_token);
  int test;
  if (test_token.ltype == NUMBER) {
    test = test_token.u.number;
  } else if (test_token.ltype == BYTE_CODES) {
    struct Token token = {UNKNOWN,{0}};
    struct Stack* local_stack = malloc(sizeof(struct Stack));
    local_stack = eval_exec_array(local_stack, test_token.u.byte_codes);
    stack_pop(&local_stack, &token);
    test = token.u.number;
  } else {
    type_error();
  }
  
  if (test) {
    main_stack = eval_exec_array(main_stack, true_token.u.byte_codes);
  } else {
    main_stack = eval_exec_array(main_stack, false_token.u.byte_codes);
  } 
  return main_stack;
}

struct Stack* while_op(struct Stack* main_stack) { 
  struct Token body_token = {UNKNOWN,{0}};
  struct Token cond_token = {UNKNOWN,{0}};
  struct Token temp_token = {UNKNOWN,{0}};
  struct Stack* local_stack = malloc(sizeof(struct Stack));

  main_stack = stack_pop(&main_stack, &body_token);
  if (body_token.ltype != BYTE_CODES) {
    printf("%d\n",cond_token.ltype);
    type_error();
  }

  main_stack = stack_pop(&main_stack, &cond_token);
  int test;
  if (cond_token.ltype == NUMBER) {
    test = cond_token.u.number;
  } else if (cond_token.ltype == BYTE_CODES) {
    main_stack = eval_exec_array(main_stack, cond_token.u.byte_codes);
    main_stack = stack_pop(&main_stack, &temp_token);
    test = temp_token.u.number;
  } else {
    type_error();
  }

  while (test) {
    main_stack = eval_exec_array(main_stack, body_token.u.byte_codes);
    main_stack = eval_exec_array(main_stack, cond_token.u.byte_codes);
    main_stack = stack_pop(&main_stack, &temp_token);
    test = temp_token.u.number;
  }
  
  return main_stack;
}

struct Stack* def_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token); 
  struct Stack def_elem = {0};
  def_elem.head.ltype = token.ltype;
  def_elem.head.u = token.u;

  main_stack = stack_pop(&main_stack, &token);
  char* def_var2 = token.u.name;
  dict_put(def_var2, &def_elem);
  return main_stack;
}
  
void register_primitives() {
  struct Stack* (*add_token)(struct Stack* stack) = add_op;
  struct Stack add_elem = {.depth = 1, .head = {ELEMENT_C_FUNC, {.cfunc = add_token}}};
  dict_put("add", &add_elem);

  struct Stack* (*def_token)(struct Stack* stack) = def_op;
  struct Stack def_elem = {.depth = 1, .head = {ELEMENT_C_FUNC, {.cfunc = def_token}}};
  dict_put("def", &def_elem);
  
  return;
}


