#include "clesson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

enum ContRead { NotReading, Reading };

struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

#define NAME_SIZE 256

int is_alpha(char c) {
  if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {return 1;} else {return 0;}
}
int is_slash(char c) {
  if (c == 47) {return 1;} else {return 0;}
}
int is_open_curly(char c) {
  if (c == 123) {return 1;} else {return 0;}
}
int is_close_curly(char c) {
  if (c == 125) {return 1;} else {return 0;}
}
int is_num(char c) {
  if (c >= 48 && c <= 57) {return 1;} else {return 0;}
}
int is_space(char c) {
  if (c == 32) {return 1;} else {return 0;}
}
int ascii_to_digit(char c) {
  return c - 48;
}

int parse_one(int prev_ch, struct Token *out_token) {
  int peek;
  int i = 0;
  enum ContRead contread = NotReading;

  if (is_num(prev_ch)) {
    out_token->u.number = ascii_to_digit(prev_ch);
    out_token->ltype = NUMBER;
  } else if (is_alpha(prev_ch)) {
    out_token->ltype = EXECUTABLE_NAME;
    out_token->u.name = (char *)malloc(NAME_SIZE * sizeof(char));
    contread = Reading;
    out_token->u.name[i] = prev_ch;
    out_token->u.name[i+1] = 0;
    i++;
  } else if (is_slash(prev_ch)) {
    out_token->ltype = LITERAL_NAME;
    out_token->u.name = (char *)malloc(NAME_SIZE * sizeof(char));
    contread = Reading;
  } else if (is_open_curly(prev_ch)) {
    out_token->ltype = OPEN_CURLY;
    out_token->u.onechar = '{';
  } else if (is_close_curly(prev_ch)) {
    out_token->ltype = CLOSE_CURLY;
    out_token->u.onechar = '}';
  } else if (is_space(prev_ch)) {
    while (is_space(peek = cl_getc())) {}
    out_token->ltype = SPACE;
    return peek;
  }

  if (out_token->ltype == UNKNOWN) {
    out_token->ltype = END_OF_FILE; 
  }

  while ((peek = cl_getc()) != EOF) {    
    if (is_space(peek)) {
      return peek;
    } else if (is_num(peek)) {
      out_token->u.number = out_token->u.number * 10 + ascii_to_digit(peek);
      out_token->ltype = NUMBER;
    } else if (is_alpha(peek)) {
      if (out_token->ltype != LITERAL_NAME) {
        out_token->ltype = EXECUTABLE_NAME;
        if (contread == NotReading) {
          contread = Reading;
          out_token->u.name = (char *)malloc(NAME_SIZE * sizeof(char));
        }
      }
      out_token->u.name[i] = peek;
      out_token->u.name[i+1] = 0;
      i++;
    } else if (is_slash(peek)) {
      out_token->ltype = LITERAL_NAME;
      if (contread == NotReading) {
        contread = Reading;
        out_token->u.name = (char *)malloc(NAME_SIZE * sizeof(char));
      }        
    } else if (is_open_curly(peek)) {
      out_token->ltype = OPEN_CURLY;
      out_token->u.onechar = '{';
    } else if (is_close_curly(peek)) {
      out_token->ltype = CLOSE_CURLY;
      out_token->u.onechar = '}';
    } else {
      out_token->ltype = UNKNOWN;
    }
  }    
  return EOF;
  
  out_token->ltype = UNKNOWN;
  return EOF;
}


void parser_print_all() {
  int ch = EOF;
  struct Token token = {
    UNKNOWN,
    {0}
  };
  
  do {
    ch = parse_one(ch, &token);
    if(token.ltype != UNKNOWN) {
      switch(token.ltype) {
      case NUMBER:
        printf("num: %d\n", token.u.number);
        break;
      case SPACE:
        printf("space!\n");
        break;
      case OPEN_CURLY:
        printf("Open curly brace '%c'\n", token.u.onechar);
        break;
      case CLOSE_CURLY:
        printf("Close curly brace '%c'\n", token.u.onechar);
        break;
      case EXECUTABLE_NAME:
        printf("EXECUTABLE_NAME: %s\n", token.u.name);
        break;
      case LITERAL_NAME:
        printf("LITERAL_NAME: %s\n", token.u.name);
        break;
        
      default:
        printf("Unknown type %d\n", token.ltype);
        break;
      }
    }
  }while(ch != EOF);
}

static void test_parse_one_123__456_7() {
  int answer1 = 0;
  char* answer2;
  char* expected = "add";
  int answer3 = 0;

  char* input = "123  add 456 7";

  struct Token token = {UNKNOWN, {0}};
  int ch = EOF;
  
  cl_getc_set_src(input);

  // parse 123
  ch = parse_one(ch, &token);
  answer1 = token.u.number;
  
  // parse spaces
  ch = parse_one(ch, &token);

  // parse add
  ch = parse_one(ch, &token);
  answer2 = token.u.name;

  // parse a space
  ch = parse_one(ch, &token);

  // parse 456
  ch = parse_one(ch, &token);
  answer3 = token.u.number;

  // verity result.
  assert(answer1 == 123);
  assert(*answer2 == *expected);
  assert(answer3 == 456);
}

static void test_parse_close_curry() {
  char* input = "}";
  char expect_char = '}';
  int expect_type = CLOSE_CURLY;

  struct Token token = {UNKNOWN, {0}};
  int ch;
  
  cl_getc_set_src(input);
  ch = parse_one(EOF, &token);
  assert(ch == EOF);
  assert(token.ltype == expect_type);
  assert(expect_char == token.u.onechar);
}

static void test_parse_open_curry() {
  char* input = "{";
  char expect_char = '{';
  int expect_type = OPEN_CURLY;

  struct Token token = {UNKNOWN, {0}};
  int ch;
  
  cl_getc_set_src(input);
  ch = parse_one(EOF, &token);
  assert(token.ltype == expect_type);
  assert(expect_char == token.u.onechar);
}

static void test_parse_literal_name() {
  char* input = "/add";
  char* expect_name = "add";
  int expect_type = LITERAL_NAME;

  struct Token token = {UNKNOWN, {0}};
  int ch;
  
  cl_getc_set_src(input);
  ch = parse_one(EOF, &token);
  assert(ch == EOF);
  assert(token.ltype == expect_type);
  assert(*expect_name == *token.u.name);
}

static void test_parse_executable_name() {
  char* input = "add";
  char* expect_name = "add";
  int expect_type = EXECUTABLE_NAME;

  struct Token token = {UNKNOWN, {0}};
  int ch;
  
  cl_getc_set_src(input);
  ch = parse_one(EOF, &token);
   
  assert(ch == EOF);
  assert(token.ltype == expect_type);
  assert(*expect_name == *token.u.name);
}

static void test_parse_one_number() {
    char *input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token); 

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE() {
    char *input = "";
    int expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}


static void unit_tests() {
  test_parse_one_empty_should_return_END_OF_FILE();
  test_parse_one_number();
  test_parse_executable_name();
  test_parse_literal_name();
  test_parse_open_curry();
  test_parse_close_curry();
}

int main() {
  unit_tests();
  test_parse_one_123__456_7();
  char* input = "123 45 add /some { 2 3 add } def";
  cl_getc_set_src(input);
  printf("%s\n",input); 
  parser_print_all();
  return 1;
}
