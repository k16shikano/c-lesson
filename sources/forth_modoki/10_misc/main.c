#include "clesson.h"
#include <stdio.h>
#include <stdlib.h>

char *input = "/factorial {dup\n%123\n%456\n {dup 1 gt} {1 sub exch 1 index mul exch} while pop} def 8 factorial";

int main(int argc, char *argv[]) {
  struct Stack init_stack = {0};
  struct Stack* result = malloc(sizeof(struct Stack));
  struct Token token = {UNKNOWN, {0}};
  FILE* source_fp;

  if (argc > 1) {
    source_fp = fopen(argv[1], "r");
    cl_set_file(source_fp);
  } else {
    cl_getc_set_src(input);
  }

  result = eval(&init_stack);
  result = stack_pop(&result, &token);
  if (token.ltype == NUMBER)
    printf("%d\n", token.u.number);

  if (source_fp) {
    fclose(source_fp);
  }

  return 0;
}
