#include "clesson.h"
#include <string.h>

static char* input = "123 456";
static int pos = 0;
void cl_gets_set_src(char* str);

int cl_getc() {
    if(strlen(input) == pos)
        return EOF;
    return input[pos++];
}

void cl_gets_set_src(char* str) {
  input = str;
  pos = 0;
}
