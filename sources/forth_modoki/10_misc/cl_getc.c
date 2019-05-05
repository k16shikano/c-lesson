#include "clesson.h"
#include <string.h>

static FILE* source_fp = NULL;
static const char* input = "123 456";
static int pos = 0;

int cl_getc() {
  if (source_fp) {
    int c = fgetc(source_fp);
    return c;
  } else if (strlen(input) == pos) {
    return EOF;
  } else {
    return input[pos++];
  }
}

void cl_getc_set_src(char* str){
  input = str;
  pos = 0;
}

void cl_set_file(FILE* fp) {
  source_fp = fp;
}
