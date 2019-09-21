#include <errno.h>   // errno
#include <stdio.h>   // fprintf, stdout
#include <stdlib.h>  // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h>  //  strerror

// #define UTF8_CONT(b) (((b >= 192) && (b <= 223)) ? b : 0 )
#define BOM_FF(b) (((b) == 0xff) ? (1) : (0))
#define BOM_FE(b) (((b) == 0xfe) ? (1) : (0))
#define UTF8_1B(b) ((((b) >> 7) == 0) ? (1) : (0))
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

int print_success(const char* path, int max_length) {
  return fprintf(stdout, "%s:%*s%s\n", path, (int)(max_length - strlen(path)),
                 " ", FILE_TYPE_STRINGS[cur_type]);
}

int print_error(const char* path, int max_length) {
  return fprintf(stdout, "%s:%*scannot open `%s' (%s)\n", path,
                 (int)(max_length - strlen(path)), " ", path, strerror(errno));
}

int getMaxLength(int nPaths, char* path[]) {
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
    unsigned char firstByte = 0;
    unsigned char nContByte = 0;

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

      // UTF-16 big or small Endian check
      if (((BOM_FF(b) == 1) || (BOM_FE(b) == 1))) {
        if ((BOM_FF(firstByte) == 1)) {
          cur_type = LEUTF16;
          break;
        } else if ((BOM_FE(firstByte) == 1)) {
          cur_type = BEUTF16;
          break;
        }
        firstByte = b;
        continue;
      }

      // UTF-8 Check
      if ((UTF8_2B(b) == 1) || (UTF8_3B(b) == 1) || (UTF8_4B(b) == 1)) {
        nContByte = 0;
        firstByte = b;
        continue;
      }
      if ((UTF8_CONT(b) == 1) && (firstByte != 0)) {
        nContByte++;
        if ((UTF8_2B(firstByte) == 1) && (nContByte == 1)) {
          cur_type = UTF8;
          break;
        } else if ((UTF8_3B(firstByte) == 1) && (nContByte == 2)) {
          cur_type = UTF8;
          break;
        } else if ((UTF8_4B(firstByte) == 1) && (nContByte == 3)) {
          cur_type = UTF8;
          break;
        }
        continue;
      }

      // ASCII
      if ((b >= 0x07 && b <= 0x0D) || b == 0x1B || (b >= 0x20 && b <= 0x7E) ||
          (b >= 0xA0)) {
        if (b >= 0xA0) {
          cur_type = ISO8859;
          break;
        } else if (cur_type != ISO8859) {
          cur_type = ASCII;
        }
      } else {
        cur_type = DATA;
        break;
      }

      i++;
      firstByte = 0;
    }
    print_success(path, max_length);
  } else {
    print_error(path, max_length);
    return;
  }
  fclose(f);
}

int main(int argc, char* argv[]) {
  int retval = EXIT_SUCCESS;
  if (argc == 1) {
    fprintf(stderr, "Usage: file path\n");
    retval = EXIT_FAILURE;
  } else {
    int max_length = getMaxLength(argc, argv) + 1;
    // Put this for loop in the getFileType function
    // Make the function retun a value and use that with the print function
    for (int i = 1; i < argc; i++) {
      getFileType(argv[i], max_length);
    }
  }
  return retval;
}
