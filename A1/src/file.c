#include <errno.h>   // errno
#include <stdio.h>   // fprintf, stdout
#include <stdlib.h>  // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h>  //  strerror

// Try not to use magic constants? Where?

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

int getMaxLength(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

void getFileType(char* filename, int max_length) {
  FILE* f = fopen(filename, "r");
  int stringLength = strlen(filename);

  if (f != NULL) {
    int i = 0;

    while (1) {
      char b;
      int read = fread(&b, 1, 1, f);

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
    fprintf(stdout, "%s:%*s%s\n", filename, (max_length - stringLength), " ",
            FILE_TYPE_STRINGS[cur_type]);
  } else {
    print_error(filename, errno);
  }
  fclose(f);
}

int main(int argc, char* argv[]) {
  int retval = EXIT_SUCCESS;
  // Check correct number of arguments
  if (argc == 1) {
    fprintf(stderr, "Usage: file path\n");
    retval = EXIT_FAILURE;
  } else {
    int max_length = 0;
    for (int i = 1; i < argc; i++) {
      max_length = getMaxLength(max_length, strlen(argv[i]));
    }
    max_length++;
    for (int i = 1; i < argc; i++) {
      getFileType(argv[i], max_length);
    }
  }
  return retval;
}
