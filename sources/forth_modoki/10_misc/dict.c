#include "clesson.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define TABLE_SIZE 1024

static int dict_pos = 0;

struct Node {
  char* key;
  struct Stack value;
  struct Node* next;
};

struct Node* dict_array[TABLE_SIZE];

int hash(char* str) {
  unsigned int val = 0;
  while(*str) {
    val += *str++;
  }
  return (int)(val%1024);
}

void update_or_insert_list(struct Node* head, char* key, struct Stack* elem) {
  while (head != NULL) {
    if (strcmp(head->key, key) == 0) {
      head->value = *elem;
      return;
    } else if (head->next == NULL) {
      head->next->key = key;
      head->next->value = *elem;
      head->next->next == NULL;
      return;
    } else {
      head = head->next;
      continue;
    }
  }
}

void dict_put(char* key, struct Stack* elem) {
  int idx = hash(key);
  struct Node* head = dict_array[idx];
  
  if (head == NULL) {
    head = malloc(sizeof(struct Node));
    head->next = NULL;
    head->key = key;
    head->value = *elem;    
    dict_array[idx] = head;
    return;
  }
  update_or_insert_list(head, key, elem);
}

int find_list(struct Node* head, char* key, struct Stack* out_elem) {
  while (head != NULL) {
    if (strcmp(head->key, key) == 0) {
      out_elem->head = head->value.head;
      out_elem->tail = head->value.tail;
      return 1;
    } else if (head->next == NULL) {
      out_elem->head = head->next->value.head;
      out_elem->tail = head->next->value.tail;
      return 1;
    } else {
      head = head->next;
      continue;
    }
  }
  return 0;
}

int dict_get(char* key, struct Stack* out_elem) {
  int r;
  int idx = hash(key);
  struct Node* head = dict_array[idx];
  
  if (head == NULL) {
    return 0;
  } else if (head->next == NULL) {
    out_elem->head = head->value.head;
    out_elem->tail = head->value.tail;
    return 1;
  } else {
    r = find_list(head, key, out_elem);
    return r;
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
  for (int i=0; i <= TABLE_SIZE; i++) {
    dict_array[i] = NULL;
  }
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
  dict_put(test_key2, &test_elems2);
  dict_put(test_key1, &test_elems2);
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
