#include <stdio.h>
#include <assert.h>

static const char* const input = "123 456  1203";

int substrtoi ();
int strtoi ();

int main() {
    int answer1 = 0;
    int answer2 = 0;
    int answer3 = 0;

    // write something here.
    answer1 = substrtoi(input, 0, 2);
    answer2 = substrtoi(input, 4, 6);
    char s3[] = {input[9],input[10],input[11],input[12],'\0'};
    answer3 = strtoi(s3);

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);

    return 1;
}

int substrtoi (char* s, int b, int e) {
  int result = 0;
  for (int i = e-b; i >= 0; --i) {
    int j = i;
    int pow = 1;
    while (j > 0) {
      pow = pow * 10;
      --j;
    }
    result = result + (s[e-i]-48) * pow;
  }
  return result;
}

int strtoi (char* s) {
  int result = 0;
  int i = 0, k = 0;
  int order = 0;
  while (s[i] != '\0') {
    ++order;
    ++i;
  }
  while (k < order) {
    int power = 1;
    int j = k;
    while (j > 0) {
      power = power * 10;
      --j;
    }
    result = result + (s[order-k-1]-48) * power;
    k++;
  }
  return result;
}

