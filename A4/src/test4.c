
#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

void put_num(const void *arg, FILE *out) {
  int num = *(int*)arg;
  fwrite(&num, sizeof(int), 1, out);
}

void minus(const void *arg, FILE *out, FILE *in) {
  int d = *(int*)arg;

  int x;
  while (fread(&x, sizeof(int), 1, in) == 1) {
    int y = x - d;
    fwrite(&y, sizeof(int), 1, out);
  }
}

void plus(const void *arg, FILE *out, FILE *in) {
  int d = *(int*)arg;

  int x;
  while (fread(&x, sizeof(int), 1, in) == 1) {
    int y = x + d;
    fwrite(&y, sizeof(int), 1, out);
  }
}

void plus_streams(const void *arg, FILE *out, FILE *in1, FILE *in2) {
  arg = arg;  // Unused
  int x, y;
  while ((fread(&x, sizeof(int), 1, in1) == 1) &&
         (fread(&y, sizeof(int), 1, in2) == 1)) {
    int z = x + y;
    fwrite(&z, sizeof(int), 1, out);
  }
}

void minus_streams(const void *arg, FILE *out, FILE *in1, FILE *in2) {
  arg = arg;  // Unused
  int x, y;
  while ((fread(&x, sizeof(int), 1, in1) == 1) &&
         (fread(&y, sizeof(int), 1, in2) == 1)) {
    int z = x - y;
    fwrite(&z, sizeof(int), 1, out);
  }
}

void print_stream(void *arg, FILE *in) {
  arg=arg; // Unused
  int x;
  while (fread(&x, sizeof(int), 1, in) == 1) {
    printf("%d\n", x);
  }
}

int main() {
  stream* s[9];

  int input1 = 50;
  int num = 5;

  assert(transducers_link_source(&s[0], put_num, &input1) == 0);
  assert(transducers_dup(&s[1], &s[2], s[0]) == 0);
  assert(transducers_dup(&s[1], &s[2], s[0]) == 1);
  assert(transducers_dup(&s[3], &s[4], s[1]) == 0);
  assert(transducers_dup(&s[3], &s[4], s[1]) == 1);
  assert(transducers_link_1(&s[5], minus, &num, s[2]) == 0);
  assert(transducers_link_1(&s[5], minus, &num, s[2]) == 1);
  assert(transducers_link_1(&s[6], plus, &num, s[3]) == 0);
  assert(transducers_link_1(&s[6], plus, &num, s[3]) == 1);
  assert(transducers_link_2(&s[7], plus_streams, NULL, s[5], s[6]) == 0);
  assert(transducers_link_2(&s[7], plus_streams, NULL, s[5], s[6]) == 1);
  assert(transducers_link_2(&s[8], minus_streams, NULL, s[7], s[4]) == 0);
  assert(transducers_link_2(&s[8], minus_streams, NULL, s[7], s[4]) == 1);
  //assert(transducers_link_sink(print_stream, NULL, s[8]) == 0);


  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }

  return 0;
}
