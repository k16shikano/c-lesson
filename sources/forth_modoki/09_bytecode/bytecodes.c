#include "clesson.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct Stack* eval(struct Stack* stack);

#define MAX_BYTE_CODES 256

struct Stack* eval_exec_array(struct Stack* main_stack, struct TokenArray* token_array) {

  int ch = EOF;
  struct Token token = {UNKNOWN,{0}};
  struct Stack* local_stack;
  struct Stack temp_elem = {0};

  for (int i = 0; i < token_array->len; i++) {
    struct Token token = token_array->elements[i];
    switch (token.ltype) {
    case NUMBER:
      local_stack = stack_push(&local_stack, token);
      break;
    case SPACE:
      break;
    case OPEN_CURLY:
      ch = compile_exec_array(ch, &token);
      local_stack = stack_push(&local_stack, token);
      break;
    case CLOSE_CURLY:
      break;
    case EXECUTABLE_NAME:
      if (strcmp(token.u.name,"add") == 0) {
        local_stack = add_op(local_stack);
      } else if (strcmp(token.u.name,"def") == 0) {
        local_stack = def_op(local_stack);
      } else {
        if (dict_get(token.u.name, &temp_elem) > 0) {
          if (temp_elem.head.ltype == ELEMENT_C_FUNC) {
            local_stack = temp_elem.head.u.cfunc(local_stack);
          } else if (temp_elem.head.ltype == BYTE_CODES) {
            local_stack = eval_exec_array(local_stack, temp_elem.head.u.byte_codes);
          } else {
            local_stack = stack_push(&local_stack, temp_elem.head);
            printf("%s\n", token.u.name);
          }
        }
      }
      break;
    case LITERAL_NAME:
      local_stack = stack_push(&local_stack, token);
      break;
      
    default:
      break;
    }
  }

  stack_pop(&local_stack, &token);
  main_stack = stack_push(&main_stack, token);
  return main_stack;
}

int compile_exec_array(int ch, struct Token* out_token) {
  struct Token temp_token = {UNKNOWN, {0}};
  struct Token temp_token_array[MAX_BYTE_CODES];
  int i = 0;
  
  do {
    ch = parse_one(ch, &temp_token);
    if (temp_token.ltype == CLOSE_CURLY) {
      
      break;
    } else if (temp_token.ltype == OPEN_CURLY) {
      ch = compile_exec_array(ch, &temp_token);
      i++;
    } else if (temp_token.ltype != SPACE) {
      temp_token_array[i].ltype = temp_token.ltype;
      temp_token_array[i].u = temp_token.u;
      i++;
   }
  } while(temp_token.ltype != CLOSE_CURLY);

  struct TokenArray* token_array = (struct TokenArray*)malloc(sizeof(struct TokenArray)) + sizeof(struct Token) * i;
  struct TokenArray byte_codes = {i,{}};
  memcpy(byte_codes.elements, temp_token_array, 3);
  out_token->ltype = BYTE_CODES;
  out_token->u.byte_codes = &byte_codes;
  
  return ch;
}

#if 1
void test_compile_exec_array() {
  struct Token token = {UNKNOWN, {0}};
  cl_getc_set_src("{1 2 3}");

  struct Stack* main_stack = malloc(sizeof(struct Stack));
  main_stack = eval(main_stack);
  stack_pop(&main_stack, &token);

  assert(3 == token.u.byte_codes->len);
  assert(2 == token.u.byte_codes->elements[1].u.number);
}

void test_compile_exec_array_1_23() {
  struct Token token = {UNKNOWN, {0}};
  cl_getc_set_src("{1} {2 3}");

  struct Stack* main_stack = malloc(sizeof(struct Stack));
  main_stack = eval(main_stack);
  stack_pop(&main_stack, &token);

  assert(2 == token.u.byte_codes->len);
  assert(3 == token.u.byte_codes->elements[1].u.number);
}

void test_compile_exec_array_1_2_3() {
  struct Token token = {UNKNOWN, {0}};
  cl_getc_set_src("{1 {2} 3}");

  struct Stack* main_stack = malloc(sizeof(struct Stack));
  main_stack = eval(main_stack);
  stack_pop(&main_stack, &token);

  assert(3 == token.u.byte_codes->len);
  assert(3 == token.u.byte_codes->elements[2].u.number);
}

void test_compile_exec_array_abc() {
  struct Token token = {UNKNOWN, {0}};
  cl_getc_set_src("{abc}");

  struct Stack* main_stack = malloc(sizeof(struct Stack));
  main_stack = eval(main_stack);
  stack_pop(&main_stack, &token);

  assert(1 == token.u.byte_codes->len);
  assert(strcmp("abc", token.u.byte_codes->elements[0].u.name) == 0);
}

void test_eval_exec_array() {
  struct Token token = {UNKNOWN, {0}};
  cl_getc_set_src("/abc {1 3 add} def abc");

  struct Stack* main_stack = malloc(sizeof(struct Stack));
  main_stack = eval(main_stack);
  stack_pop(&main_stack, &token);

  assert(4 == token.u.number);

}

void unit_tests() {
  test_compile_exec_array_1_2_3();
  test_compile_exec_array_1_23();
  test_compile_exec_array_abc();
  test_compile_exec_array();
  test_eval_exec_array();
}

int main() {
  unit_tests();
  return 1;
}
#endif
