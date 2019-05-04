#include "clesson.h"

struct Stack* add_op(struct Stack* main_stack) {
  struct Token token = {UNKNOWN,{0}};
  main_stack = stack_pop(&main_stack, &token);
  int var1 = token.u.number;
  main_stack = stack_pop(&main_stack, &token);
  int var2 = token.u.number;
  int val = var1 + var2;
  token.u.number = val;
  main_stack = stack_push(&main_stack, token);
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


