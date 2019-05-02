#include "clesson.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DICT_SIZE 1024

static int dict_pos = 0;
struct KeyValue {
  char *key;
  struct Stack value;
};
static struct KeyValue dict_array[DICT_SIZE] = {};

int find_dict(char* key) {
  int i = 0;
  
  for (i; i < dict_pos; ++i) {
    if (strcmp(dict_array[i].key, key) == 0) {return i;} else {continue;}
  }
  return -1;
}

void dict_put(char* key, struct Stack* elems) {
  struct KeyValue entry = {0};
  entry.key = key;
  entry.value = *elems;
  int i;
  
  if ((i = find_dict(key)) < 0) { // key doesn't exist.
    dict_array[dict_pos] = entry;
    dict_pos++;
  } else {                        // key exists.
    dict_array[i] = entry;
  }
}

int dict_get(char* key, struct Stack* out_elem) {
  int i;
  if ((i = find_dict(key)) < 0) { // key doesn't exist.
    return 0;
  } else {                        // key exists.
    out_elem->head = dict_array[i].value.head;
    out_elem->tail = dict_array[i].value.tail;
    return 1;
  }
}

#if 0
/* setup dict for test */
static char* test_key1 = "x";
static char* test_key2 = "y";
static const struct Token test_token1 = {NUMBER, {.number = 42}};
static const struct Token test_token2 = {NUMBER, {.number = 11}};
static struct Stack test_elems1 = {.depth = 1, .head = test_token1};
static struct Stack test_elems2 = {.depth = 1, .head = test_token2};

void clear_dict() {
  struct KeyValue null_elem = {0};
  for (int i=0; i <= dict_pos; i++) {  
    dict_array[i] = null_elem;
  }
  dict_pos = 0;
}

void test_put_get () {
  clear_dict();
  
  struct Stack elem = {0};
  int r;

  dict_put(test_key2, &test_elems2);
  dict_put(test_key1, &test_elems1);
  r = dict_get(test_key1, &elem);

  assert(r == 1);
  assert(elem.head.u.number == 42);
}

void test_get () {
  clear_dict();
  struct Stack elem;
  int r;
  r = dict_get(test_key1, &elem);

  assert(r == 0);
}

void test_put () {
  clear_dict();

  dict_put(test_key1, &test_elems1);
  assert(dict_array[0].value.head.ltype == NUMBER);
  assert(dict_array[0].value.head.u.number == 42);

  dict_put(test_key2, &test_elems2);
  assert(dict_array[1].value.head.ltype == NUMBER);
  assert(dict_array[1].value.head.u.number == 11);

  dict_put(test_key1, &test_elems2);
  assert(dict_array[0].value.head.ltype == NUMBER);
  assert(dict_array[0].value.head.u.number == 11);
}

static void unit_tests() {
  test_put();
  test_get();
  test_put_get();
}

int main() {
  unit_tests();
  return 1;
}

#endif
