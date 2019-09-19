#include <errno.h>   // errno
#include <stdio.h>   // fprintf, stdout
#include <stdlib.h>  // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h>  //  strerror

// #define UTF8_CONT(b) (((b >= 192) && (b <= 223)) ? b : 0 )
#define UTF8_CONT(b) ((((b) >> 6) == 2) ? (1) : (0))
#define UTF8_2B(b) ((((b) >> 5) == 6) ? (1) : (0))
#define UTF8_3B(b) ((((b) >> 4) == 14) ? (1) : (0))
#define UTF8_4B(b) ((((b) >> 3) == 30) ? (1) : (0))

enum file_type {
  DATA,
  EMPTY,
  ASCII,
  ISO8859,
  UTF8,
  LEUTF16,
  BEUTF16,
};

const char* const FILE_TYPE_STRINGS[] = {
    "data",
    "empty",
    "ASCII text",
    "ISO-8859 text",
    "UTF-8 Unicode text",
    "Little-endian UTF-16 Unicode text",
    "Big-endian UTF-16 Unicode text",
};

enum file_type cur_type;

int print_error(const char* path, int max_length, int errnum) {
  return fprintf(stdout, "%s:%*scannot determine (%s)\n", path,
                 (int)(max_length - strlen(path)), " ", strerror(errnum));
}

int getMaxLength2(int nPaths, char* path[]) {
  int max_length = 0;
  for (int i = 1; i < nPaths; i++) {
    int stringLength = strlen(path[i]);
    if (stringLength > max_length) {
      max_length = stringLength;
    }
  }
  return max_length;
}

void getFileType(char* path, int max_length) {
  FILE* f = fopen(path, "r");

  if (f != NULL) {
    int i = 0;

    while (1) {
      unsigned char b;
      int read = fread(&b, 1, 1, f);

      // Exit loop if file read is done
      if (read == 0 && i != 0) {
        break;
      }
      // Empty check
      if (read == 0 && i == 0) {
        cur_type = EMPTY;
        break;
      }

      if ((b >= 0x07 && b <= 0x0D) || b == 0x1B || (b >= 0x20 && b <= 0x7E)) {
        cur_type = ASCII;
      } else if ((b >= 0x07 && b <= 0x0D) || b == 0x1B ||
                 (b >= 0x20 && b <= 0x7E) || (b >= 0xA0)) {
        cur_type = ISO8859;
        break;
      } else {
        cur_type = DATA;
        break;
      }

      i++;
    }
    fprintf(stdout, "%s:%*s%s\n", path, (int)(max_length - strlen(path)), " ",
            FILE_TYPE_STRINGS[cur_type]);
  } else {
    print_error(path, max_length, errno);
  }
  fclose(f);
}

int main(int argc, char* argv[]) {
  int retval = EXIT_SUCCESS;
  if (argc == 1) {
    fprintf(stderr, "Usage: file path\n");
    retval = EXIT_FAILURE;
  } else {
    int max_length = getMaxLength2(argc, argv) + 1;
    // Put this for loop in the getFileType function
    for (int i = 1; i < argc; i++) {
      getFileType(argv[i], max_length);
    }
  }
  return retval;
}
