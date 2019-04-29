#include "clesson.h"
#include <assert.h>

/*
cc cl_getc.c int_parser_getc.c
*/

enum PS {NUMBER, SPACE};

int parse_one();
void test_parse_one_123();
void test_parse_one_123__456_7();

int main() {
    test_parse_one_123();
    test_parse_one_123__456_7();

    return 1;
}

int parse_one(int peeked, int *out_val, int *out_type) {
  int c;
  *out_val = (peeked==0) ? 0 : peeked - 48;

  if (c == 32) {
    while ((c = cl_getc()) == 32) {}
    *out_val = 0;
    *out_type = SPACE;
    return c;
  } else {
    while ((c = cl_getc()) != EOF) {
      if (c == 32) {
        *out_type = SPACE;
        return c;
      } else {
        *out_val = *out_val * 10 + (c - 48);
        *out_type = NUMBER;
      }
    }
  }
  return 0;
}

void test_parse_one_123() {
  int out_val;
  int out_type;
  int c = '\0';
  int answer = 0;
  
  cl_gets_set_src("123");
  c = parse_one(c, &out_val, &out_type);
  assert(out_val == 123);
}

void test_parse_one_123__456_7() {
  int out_val;
  int out_type;
  int c = '\0';
  int answer1 = 0;
  int answer2 = 0;
  int answer3 = 0;

  cl_gets_set_src("123  456 7");

  // parse 123
  c = parse_one(c, &out_val, &out_type);
  answer1 = out_val;

  // parse spaces
  c = parse_one(c, &out_val, &out_type);

  // parse 456
  c = parse_one(c, &out_val, &out_type);
  answer2 = out_val;

  // parse a space
  c = parse_one(c, &out_val, &out_type);

  // parse 9
  c = parse_one(c, &out_val, &out_type);
  answer3 = out_val;

  // verity result.
  assert(answer1 == 123);
  assert(answer2 == 456);
  assert(answer3 == 7);
}
