#include <errno.h>   // errno
#include <stdio.h>   // fprintf, stdout
#include <stdlib.h>  // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h>  //  strerror

enum file_type {
  DATA,
  EMPTY,
  ASCII,
};

const char* const FILE_TYPE_STRINGS[] = {
    "data",
    "empty",
    "ASCII text",
};

enum file_type cur_type;

int print_error(char* path, int errnum) {
  return fprintf(stdout, "%s: Cannot determine (%s)\n", path, strerror(errnum));
}

int print_hello_world() { return fprintf(stdout, "Hello, world!\n"); }

void getFileType(FILE* file) {
  int i = 0;
  while (1) {
    char b;
    int read = fread(&b, 1, 1, file);

    if (read == 0 && i == 0) {
      cur_type = EMPTY;
      break;
    } else if (read == 0) {
      break;
    } else if ((b >= 0x07 && b <= 0x0D) || b == 0x1B ||
               (b >= 0x20 && b <= 0x7E)) {
      cur_type = ASCII;
    } else {
      cur_type = DATA;
      break;
    }

    i++;
  }
}

int main(int argc, char* argv[]) {
  int retval = EXIT_SUCCESS;
  // Check correct number of arguments
  if (argc == 1) {
    fprintf(stderr, "Usage: file path\n");
    retval = EXIT_FAILURE;
  } else if (argc > 2) {
    fprintf(stderr, "To many arguments given\n");
    retval = EXIT_FAILURE;
  } else {
    char* filename = argv[1];

    FILE* f = fopen(filename, "r");

    if (f != NULL) {
      getFileType(f);
    } else {
      fclose(f);
      print_error(argv[1], errno);
      retval = EXIT_FAILURE;
      return retval;
    }

    fprintf(stdout, "%s: %s\n", filename, FILE_TYPE_STRINGS[cur_type]);
  }
  return retval;
}
