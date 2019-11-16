#include "transducers.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void string_stream(const void *arg, FILE *out) {
  fputs((const char *)arg, out);
}

void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;

  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }
}

int main() {
  stream* s[3];

  char *input = "Hello, World!";
  char *output1 = malloc(strlen(input) + 1);
  char *output2 = malloc(strlen(input) + 1);
  output1[strlen(input)] = '\0'; /* Ensure terminating NULL. */
  output2[strlen(input)] = '\0'; /* Ensure terminating NULL. */

  assert(transducers_link_source(&s[0], string_stream, input) == 0);
  assert(transducers_dup(&s[1], &s[2], s[0]) == 0);
  assert(transducers_link_sink(save_stream, output1, s[1]) == 0);
  assert(transducers_link_sink(save_stream, output2, s[2]) == 0);

  /* We cannot use the '==' operator for comparing strings, as strings
     in C are just pointers.  Using '==' would compare the _addresses_
     of the two strings, which is not what we want. */
  assert(strcmp(input, output1) == 0);
  assert(strcmp(input, output2) == 0);

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s) / sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }

  free(output1);
  free(output2);

  return 0;
}
