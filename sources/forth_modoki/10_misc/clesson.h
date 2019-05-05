#include <stdio.h>

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();
void cl_getc_set_src(char* str);
void cl_set_file(FILE* fp);

/*
parser
*/
enum LexicalType {
  NUMBER,
  SPACE,
  EXECUTABLE_NAME,
  LITERAL_NAME,
  OPEN_CURLY,
  CLOSE_CURLY, 
  END_OF_FILE,
  ELEMENT_C_FUNC,
  BYTE_CODES,
  UNKNOWN,
};

struct Token {
  enum LexicalType ltype;
  union {
    int number;
    char onechar;
    char* name;
    struct Stack* (*cfunc)();
    struct TokenArray* byte_codes;
  } u;
};  

int parse_one(int prev_ch, struct Token *out_token);

/*
byte code
*/

struct TokenArray {
  int len;
  struct Token elements[0];
};

struct Stack* eval_exec_array(struct Stack* stack, struct TokenArray* out_bytecodes);
int compile_exec_array(int ch, struct Token* out_token);

/*
stack
*/
struct Stack {
  int depth;
  struct Token head;
  struct Stack* tail;
};

int stack_init(struct Stack** stack);
struct Stack* stack_pop(struct Stack** stack, struct Token* out_token);
struct Stack* stack_push(struct Stack** stack, struct Token token);

/*
dict
*/

int dict_get(char* key, struct Stack* out_elem);
void dict_put(char* key, struct Stack* elems);

/*
primitives
*/

struct Stack* add_op(struct Stack* main_stack);
struct Stack* sub_op(struct Stack* main_stack);
struct Stack* mul_op(struct Stack* main_stack);
struct Stack* div_op(struct Stack* main_stack);
struct Stack* eq_op(struct Stack* main_stack);
struct Stack* neq_op(struct Stack* main_stack);
struct Stack* gt_op(struct Stack* main_stack);
struct Stack* ge_op(struct Stack* main_stack);
struct Stack* lt_op(struct Stack* main_stack);
struct Stack* le_op(struct Stack* main_stack);
struct Stack* def_op(struct Stack* main_stack);
struct Stack* pop_op(struct Stack* main_stack);
struct Stack* exch_op(struct Stack* main_stack);
struct Stack* dup_op(struct Stack* main_stack);
struct Stack* index_op(struct Stack* main_stack);
struct Stack* roll_op(struct Stack* main_stack);
struct Stack* exec_op(struct Stack* main_stack);
struct Stack* if_op(struct Stack* main_stack);
struct Stack* ifelse_op(struct Stack* main_stack);
struct Stack* while_op(struct Stack* main_stack);

void register_primitives();
  
/*
eval
*/
struct Stack* eval(struct Stack* stack);

