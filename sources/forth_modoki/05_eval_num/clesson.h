#include <stdio.h>

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();
void cl_getc_set_src(char* str);


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
    UNKNOWN
};

struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};  

int parse_one(int prev_ch, struct Token *out_token);

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
